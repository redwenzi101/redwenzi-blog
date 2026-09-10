// 评论组件（Giscus）：仅在配置完整时加载，未配置时静默跳过
(function () {
  document.addEventListener("DOMContentLoaded", function () {
    var cfg = window.GISCUS_CONFIG || {};
    var art = document.querySelector("article");
    if (!art) return;
    if (!cfg.repo || !cfg.repoId || !cfg.categoryId) return; // 未配置 → 优雅降级

    var box = document.createElement("section");
    box.className = "comments";
    box.innerHTML = '<h2>💬 评论</h2><div class="giscus"></div>';
    art.appendChild(box);

    var s = document.createElement("script");
    s.src = "https://giscus.app/client.js";
    s.async = true;
    s.crossOrigin = "anonymous";
    s.setAttribute("data-repo", cfg.repo);
    s.setAttribute("data-repo-id", cfg.repoId);
    s.setAttribute("data-category", cfg.category || "Announcements");
    s.setAttribute("data-category-id", cfg.categoryId);
    s.setAttribute("data-mapping", cfg.mapping || "pathname");
    s.setAttribute("data-reactions-enabled", "1");
    s.setAttribute("data-emit-metadata", "0");
    s.setAttribute("data-input-position", "bottom");
    s.setAttribute("data-theme", "light");
    s.setAttribute("data-lang", "zh-CN");
    box.querySelector(".giscus").appendChild(s);
  });
})();
