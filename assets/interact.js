// 互动组件：回到顶部 + 点赞 + 导航滚动模糊/当前页高亮
document.addEventListener("DOMContentLoaded", function () {
  // 回到顶部
  var top = document.createElement("button");
  top.className = "backtop";
  top.textContent = "↑";
  top.title = "回到顶部";
  top.addEventListener("click", function () { window.scrollTo({ top: 0, behavior: "smooth" }); });
  document.body.appendChild(top);

  // 导航滚动状态 + 当前页高亮
  var nav = document.querySelector("nav");
  window.addEventListener("scroll", function () {
    var y = window.scrollY;
    top.style.display = y > 300 ? "flex" : "none";
    if (nav) nav.classList.toggle("nav-scrolled", y > 50);
  }, { passive: true });

  var here = location.pathname.split("/").pop() || "index.html";
  document.querySelectorAll("nav a").forEach(function (a) {
    var target = a.getAttribute("href") || "";
    if (target.split("/").pop() === here) a.classList.add("active");
  });

  // 点赞（仅本站演示）
  var art = document.querySelector("article");
  if (art) {
    var key = "like-" + location.pathname;
    var n = parseInt(localStorage.getItem(key) || "0", 10);
    var box = document.createElement("div");
    box.className = "likebar";
    box.innerHTML =
      '<button class="like" aria-label="喜欢">👍 有用&nbsp;<span class="cnt">' + n + "</span></button>" +
      "<small>仅本站演示（无后端，跨用户不计）</small>";
    art.appendChild(box);
    box.querySelector("button").addEventListener("click", function () {
      n += 1;
      localStorage.setItem(key, n);
      box.querySelector(".cnt").textContent = n;
    });
  }
});

// ===== 灵动岛：全站悬浮胶囊 =====
// 折叠态：站点状态点 + 当前页面名 + 阅读进度
// 展开态：正在做 + 进度条 + 六个入口 + 回到顶部 / 搜索
// 有吸顶导航的页面：滚过阈值后导航上滑收起，胶囊接管入口；回到顶部再换回来。
// 单个页面想关掉：给 <html> 加 data-island="off"。
(function () {
  document.addEventListener("DOMContentLoaded", function () {
    if (document.documentElement.getAttribute("data-island") === "off") return;
    if (document.querySelector(".island")) return;

    var SHOW_AT = 150; // 滚动超过这个像素才出现（页面顶部保持干净的标题区）
    var LINKS = [
      ["/", "⌂ 首页"],
      ["/roadmap/", "🗺️ 学习路线"],
      ["/programming/dsa/", "🧮 数据结构与算法"],
      ["/programming/", "💻 编程语言"],
      ["/knowledge/", "📚 知识大全"],
      ["/index.html#blog", "✍️ 博客"],
      ["/index.html#projects", "🧪 项目"],
      ["/index.html#about", "🙋 关于"]
    ];

    var nav = document.querySelector("nav");
    var h1 = document.querySelector("h1");
    var label = (h1 ? h1.textContent : document.title).replace(/\s+/g, " ").trim();
    if (label.length > 18) label = label.slice(0, 18) + "…";

    var island = document.createElement("div");
    island.className = "island";
    island.setAttribute("role", "region");
    island.setAttribute("aria-label", "站点快捷面板");
    island.innerHTML =
      '<button class="island-head" type="button" aria-expanded="false" aria-label="展开站点快捷面板">' +
        '<span class="island-dot"></span>' +
        '<span class="island-label"></span>' +
        '<span class="island-pct">0%</span>' +
        '<span class="island-caret">▼</span>' +
      '</button>' +
      '<div class="island-body">' +
        '<div class="island-live">🔄 正在做：<b>数据结构与算法</b> · 已上线 2 章（链表 / 复杂度）</div>' +
        '<div class="island-track"><i></i></div>' +
        '<div class="island-links">' +
          LINKS.map(function (l) { return '<a href="' + l[0] + '">' + l[1] + "</a>"; }).join("") +
        '</div>' +
        '<div class="island-actions">' +
          '<button type="button" class="island-top">↑ 回到顶部</button>' +
          '<button type="button" class="island-search">🔍 搜索</button>' +
        '</div>' +
      '</div>';
    island.querySelector(".island-label").textContent = label;
    document.body.appendChild(island);

    var head = island.querySelector(".island-head");
    var pctEl = island.querySelector(".island-pct");
    var barEl = island.querySelector(".island-track i");

    function setOpen(open) {
      island.classList.toggle("open", open);
      head.setAttribute("aria-expanded", open ? "true" : "false");
    }

    head.addEventListener("click", function (e) {
      e.stopPropagation();
      setOpen(!island.classList.contains("open"));
    });
    island.addEventListener("click", function (e) { e.stopPropagation(); });
    island.querySelectorAll("a").forEach(function (a) {
      a.addEventListener("click", function () { setOpen(false); });
    });
    island.querySelector(".island-top").addEventListener("click", function () {
      setOpen(false);
      window.scrollTo({ top: 0, behavior: "smooth" });
    });
    island.querySelector(".island-search").addEventListener("click", function () {
      setOpen(false);
      var toggle = document.querySelector(".search-toggle");
      if (toggle) toggle.click();
    });
    document.addEventListener("click", function () { setOpen(false); });
    document.addEventListener("keydown", function (e) { if (e.key === "Escape") setOpen(false); });

    function update() {
      var y = window.scrollY || document.documentElement.scrollTop || 0;
      var max = document.documentElement.scrollHeight - window.innerHeight;
      var pct = max > 20 ? Math.min(100, Math.round((y / max) * 100)) : 100;
      pctEl.textContent = pct + "%";
      barEl.style.width = pct + "%";
      var active = y > SHOW_AT;
      if (!active) setOpen(false);
      island.classList.toggle("on", active);
      if (nav) nav.classList.toggle("nav-hidden", active);
    }

    var ticking = false;
    window.addEventListener("scroll", function () {
      if (ticking) return;
      ticking = true;
      requestAnimationFrame(function () { update(); ticking = false; });
    }, { passive: true });

    update();
  });
})();
