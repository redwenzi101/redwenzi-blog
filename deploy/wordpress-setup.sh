#!/usr/bin/env bash
# 把服务器上已有的 WordPress 跑起来，挂在静态站的 /blog 子目录下。
# 静态站（现在这套首页设计）保持不动，WordPress 只作为动态部分 / 后台。
#
# 用法：
#   bash wordpress-setup.sh                        # 默认域名 redwenzi.icu，子目录 blog
#   bash wordpress-setup.sh redwenzi.icu blog

set -euo pipefail

DOMAIN="${1:-redwenzi.icu}"
SUB="${2:-blog}"
SITE_ROOT="/var/www/redwenzi-blog"     # 静态站根目录
WP_SRC="/var/www/redwenzi"             # 现在放着 WordPress 文件的目录
WP_DIR="${SITE_ROOT}/${SUB}"
DB_NAME="blog"
DB_USER="redwenzi"
CONF="/etc/nginx/conf.d/${DOMAIN%%.*}.conf"

if [ "$(id -u)" -ne 0 ]; then
  echo "❌ 请用 root 运行（或加 sudo）"
  exit 1
fi

echo "→ [1/6] 安装 PHP 与数据库组件（已装的会跳过）"
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get install -y php-fpm php-mysql php-curl php-gd php-mbstring php-xml php-zip

if ! command -v mariadbd >/dev/null 2>&1 && ! command -v mysqld >/dev/null 2>&1; then
  echo "   没检测到数据库，安装 MariaDB"
  apt-get install -y mariadb-server
fi
systemctl enable --now mariadb >/dev/null 2>&1 || systemctl enable --now mysql >/dev/null 2>&1 || true

PHPV="$(ls /etc/php 2>/dev/null | sort -V | tail -1)"
if [ -z "$PHPV" ]; then echo "❌ 没找到 PHP，安装可能失败了"; exit 1; fi
systemctl enable --now "php${PHPV}-fpm" >/dev/null 2>&1 || true
PHP_SOCK="/run/php/php${PHPV}-fpm.sock"
echo "   PHP 版本：$PHPV，socket：$PHP_SOCK"

echo "→ [2/6] 建数据库与用户"
DB_PASS="$(tr -dc 'A-Za-z0-9' < /dev/urandom | head -c 16)"
mysql -e "CREATE DATABASE IF NOT EXISTS ${DB_NAME} DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;"
mysql -e "CREATE USER IF NOT EXISTS '${DB_USER}'@'localhost' IDENTIFIED BY '${DB_PASS}';"
mysql -e "ALTER USER '${DB_USER}'@'localhost' IDENTIFIED BY '${DB_PASS}';"
mysql -e "GRANT ALL PRIVILEGES ON ${DB_NAME}.* TO '${DB_USER}'@'localhost'; FLUSH PRIVILEGES;"

echo "→ [3/6] 把 WordPress 文件放到 ${WP_DIR}"
mkdir -p "$SITE_ROOT" "$WP_DIR"
if [ -f "${WP_SRC}/wp-load.php" ]; then
  cp -a "${WP_SRC}/." "$WP_DIR/"
  echo "   已从 ${WP_SRC} 复制（原目录保留，没删）"
elif [ ! -f "${WP_DIR}/wp-load.php" ]; then
  echo "❌ ${WP_SRC} 和 ${WP_DIR} 里都没找到 WordPress 文件（缺 wp-load.php）"
  exit 1
fi

echo "→ [4/6] 生成 wp-config.php（随机盐由本机生成）"
if [ ! -f "${WP_DIR}/wp-config.php" ]; then
  gen() { tr -dc 'A-Za-z0-9!@#$%^&*()_+-=[]{}' < /dev/urandom | head -c 48; }
  cat > "${WP_DIR}/wp-config.php" <<PHPEOF
<?php
define('DB_NAME', '${DB_NAME}');
define('DB_USER', '${DB_USER}');
define('DB_PASSWORD', '${DB_PASS}');
define('DB_HOST', 'localhost');
define('DB_CHARSET', 'utf8mb4');
define('DB_COLLATE', '');

define('AUTH_KEY',         '$(gen)');
define('SECURE_AUTH_KEY',  '$(gen)');
define('LOGGED_IN_KEY',    '$(gen)');
define('NONCE_KEY',        '$(gen)');
define('AUTH_SALT',        '$(gen)');
define('SECURE_AUTH_SALT', '$(gen)');
define('LOGGED_IN_SALT',   '$(gen)');
define('NONCE_SALT',       '$(gen)');

\$table_prefix = 'wp_';

define('WP_DEBUG', false);

if ( ! defined('ABSPATH') ) {
    define('ABSPATH', __DIR__ . '/');
}

require_once ABSPATH . 'wp-settings.php';
PHPEOF
  echo "   已生成"
else
  echo "   wp-config.php 已存在，跳过（如需改数据库密码请手动改）"
fi

chown -R www-data:www-data "$WP_DIR"

echo "→ [5/6] 给 nginx 加 /${SUB} 的 PHP 处理"
mkdir -p /etc/nginx/snippets
cat > /etc/nginx/snippets/wp-php.conf <<NGINXEOF
    location = /${SUB} { return 301 /${SUB}/; }
    location /${SUB}/ {
        try_files \$uri \$uri/ /${SUB}/index.php?\$args;
    }
    location ~ \.php\$ {
        include snippets/fastcgi-php.conf;
        fastcgi_pass unix:${PHP_SOCK};
    }
NGINXEOF

if [ ! -f "$CONF" ]; then
  echo "   ⚠️ 没找到 $CONF，请确认静态站的 nginx 配置文件名"
  exit 1
fi
if ! grep -q 'wp-php.conf' "$CONF"; then
  sed -i "/server_name/a\\    include snippets/wp-php.conf;" "$CONF"
  echo "   已把 include 写进 $CONF"
else
  echo "   已经 include 过了，跳过"
fi

nginx -t
systemctl reload nginx

echo "→ [6/6] 完成 ✅"
echo ""
echo "   打开 https://${DOMAIN}/${SUB}/ 完成 WordPress 安装"
echo "   （站点标题、管理员用户名/密码、邮箱，都在浏览器里填）"
echo ""
echo "   数据库信息（已写进 wp-config.php，建议记下来）："
echo "     库名：${DB_NAME}"
echo "     用户：${DB_USER}"
echo "     密码：${DB_PASS}"
echo ""
echo "   静态首页仍然是 https://${DOMAIN}/ ，没动。"
