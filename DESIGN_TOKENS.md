# 设计 Token 清单（备案后移植 WordPress 用）

所有颜色/圆角/阴影/渐变集中在 `assets/theme.css` 的 `:root`，组件只引用变量，便于迁移到 WordPress 主题。

| 变量 | 用途 | 值 |
|---|---|---|
| `--bg` | 页面底色（奶油米黄） | `#faf7f2` |
| `--card` | 卡片背景 | `#ffffff` |
| `--card-hover` | 卡片 hover 背景 | `#fefcf8` |
| `--text` | 正文 | `#2b2b2b` |
| `--text-soft` | 次要文字 | `#666666` |
| `--text-muted` | 弱化文字 | `#999999` |
| `--sub` | 说明文字 | `#6a6a6a` |
| `--accent` | 主强调（钢笔墨蓝） | `#1f3a5f` |
| `--accent-light` | 主强调浅 | `#3a5a8c` |
| `--accent-lighter` | 主强调极浅 | `#e8eef5` |
| `--accent-dark` | 主强调深 | `#152a45` |
| `--accent-gold` | 次强调（旧书烫金） | `#c9a227` |
| `--accent-gold-light` | 烫金浅 | `#e0c255` |
| `--accent-gold-lighter` | 烫金极浅 | `#faf3e0` |
| `--accent-gold-dark` | 烫金深 | `#a0801a` |
| `--border` | 暖灰边框 | `#ece6da` |
| `--border-light` | 更淡边框 | `#f2ede3` |
| `--code-bg` | 代码块底 | `#1f2430` |
| `--code-text` | 代码文字 | `#e8e6e3` |
| `--radius` / `--radius-sm` / `--radius-lg` | 圆角 | `12px` / `8px` / `16px` |
| `--shadow` | 卡片阴影 | `0 1px 3px rgba(0,0,0,.04), 0 4px 12px rgba(0,0,0,.04)` |
| `--shadow-hover` | hover 阴影 | `0 4px 12px rgba(0,0,0,.08), 0 8px 24px rgba(0,0,0,.06)` |
| `--shadow-inner` | 顶部内高光 | `inset 0 1px 0 rgba(255,255,255,.8)` |
| `--gradient-title` | 标题渐变 | `linear-gradient(135deg,#1f3a5f,#c9a227)` |
| `--gradient-divider` | 分隔线渐变 | `linear-gradient(90deg,transparent,#c9a227,transparent)` |
| `--gradient-bg` | 背景渐变 | `radial-gradient(ellipse at top,#fdfaf5,#faf7f2 50%,#f5f0e8)` |

## 字体

- 标题：`"Noto Serif SC","Source Han Serif SC","Songti SC",Georgia,serif`（字重 600 / h3 500）
- 正文：`-apple-system, "PingFang SC", "Microsoft YaHei", sans-serif`，17px / 行高 1.8

## 组件清单

| 组件 | 文件 | 说明 |
|---|---|---|
| 主题变量/组件样式 | `assets/theme.css` | 全站统一 |
| 文章增强（目录/高亮/行号/复制/入场） | `assets/article.js` | 仅文章页 |
| 语法高亮 | `assets/highlight.min.js` | 本地托管，离线可用 |
| 互动（回顶部/点赞/导航滚动） | `assets/interact.js` | 全站 |
| 评论（Giscus） | `assets/comments.js` + `giscus-config.js` | 需填 repo-id 后启用 |
| 搜索（Pagefind） | 待构建索引 | 见执行报告待办 |
