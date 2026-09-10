// 全文搜索（Pagefind）：右上角搜索按钮 + 结果下拉；离线/无索引时优雅降级
(function () {
  document.addEventListener("DOMContentLoaded", function () {
    var btn = document.createElement("button");
    btn.className = "search-toggle";
    btn.textContent = "🔍";
    btn.title = "搜索（按 / 快速打开）";
    var panel = document.createElement("div");
    panel.className = "search-panel";
    panel.innerHTML = '<input class="search-input" type="search" placeholder="搜索文章…"><div class="search-results"></div>';
    document.body.appendChild(btn);
    document.body.appendChild(panel);

    var engine = null, loaded = false;
    async function ensure() {
      if (loaded) return engine;
      loaded = true;
      try {
        engine = await import("/_pagefind/pagefind.js");
        await engine.init();
      } catch (e) { engine = null; }
      return engine;
    }

    btn.addEventListener("click", function () {
      panel.classList.toggle("open");
      if (panel.classList.contains("open")) panel.querySelector("input").focus();
    });
    document.addEventListener("keydown", function (e) {
      if (e.key === "/" && document.activeElement.tagName !== "INPUT" && document.activeElement.tagName !== "TEXTAREA") {
        e.preventDefault(); panel.classList.add("open"); panel.querySelector("input").focus();
      }
      if (e.key === "Escape") panel.classList.remove("open");
    });

    var input = panel.querySelector(".search-input");
    var out = panel.querySelector(".search-results");
    var timer = null;
    input.addEventListener("input", function () { clearTimeout(timer); timer = setTimeout(run, 220); });

    async function run() {
      var q = input.value.trim();
      out.innerHTML = "";
      if (!q) return;
      var eng = await ensure();
      if (!eng) { out.innerHTML = '<p class="sr-empty">搜索需在线访问（本地预览不可用）</p>'; return; }
      var r = await eng.search(q);
      var top = r.results.slice(0, 6);
      if (!top.length) { out.innerHTML = '<p class="sr-empty">没有找到结果</p>'; return; }
      for (var i = 0; i < top.length; i++) {
        var d = await top[i].data();
        var a = document.createElement("a");
        a.className = "sr-item";
        a.href = d.url;
        a.innerHTML = "<b>" + (d.meta && d.meta.title ? d.meta.title : d.url) + "</b><p>" + (d.excerpt || "") + "</p>";
        out.appendChild(a);
      }
    }
  });
})();
