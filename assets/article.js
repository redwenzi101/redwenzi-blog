// 文章统一增强：语法高亮 + 侧边行号 + 复制按钮 + 文内目录 + 滚动入场
document.addEventListener("DOMContentLoaded", function () {
  var article = document.querySelector("article");

  // 0) 语法高亮（本地 highlight.js，离线可用）
  if (window.hljs) {
    document.querySelectorAll("pre code").forEach(function (code) {
      try { window.hljs.highlightElement(code); } catch (e) {}
    });
  }

  // 1) 侧边行号（不破坏高亮补丁：用独立 gutter）
  document.querySelectorAll("pre").forEach(function (pre) {
    var code = pre.querySelector("code");
    if (!code) return;
    var lines = code.innerText.replace(/\n$/, "").split("\n").length;
    if (lines < 2) return;
    var gut = document.createElement("div");
    gut.className = "ln-gutter";
    var nums = [];
    for (var i = 1; i <= lines; i++) nums.push(i);
    gut.textContent = nums.join("\n");
    pre.appendChild(gut);
  });

  // 2) 复制按钮（复制纯代码，不含行号）
  document.querySelectorAll("pre").forEach(function (pre) {
    if (pre.querySelector(".copy-btn")) return;
    var btn = document.createElement("button");
    btn.className = "copy-btn";
    btn.textContent = "复制";
    btn.addEventListener("click", function () {
      var code = pre.querySelector("code");
      var text = code ? code.innerText.replace(/\n$/, "") : "";
      navigator.clipboard.writeText(text).then(function () {
        btn.textContent = "已复制";
        setTimeout(function () { btn.textContent = "复制"; }, 1500);
      });
    });
    pre.appendChild(btn);
  });

  // 3) 文内目录（从 h2 自动生成）
  if (article) {
    var heads = Array.prototype.slice.call(article.querySelectorAll("h2"));
    if (heads.length) {
      var box = document.createElement("div");
      box.className = "toc-inline";
      box.innerHTML = "<b>📖 本文目录</b><ol></ol>";
      var list = box.querySelector("ol");
      heads.forEach(function (h, i) {
        if (!h.id) h.id = "sec-" + (i + 1);
        var li = document.createElement("li");
        var a = document.createElement("a");
        a.href = "#" + h.id;
        a.textContent = h.textContent;
        li.appendChild(a);
        list.appendChild(li);
      });
      article.insertBefore(box, article.firstChild);
    }
  }

  // 4) 滚动入场动效（错落延迟）
  if (article && "IntersectionObserver" in window) {
    var blocks = article.querySelectorAll("h2, h3, p, pre, table, ul, ol, .tip, .warn, .block");
    var io = new IntersectionObserver(function (entries) {
      entries.forEach(function (e) {
        if (e.isIntersecting) { e.target.classList.add("visible"); io.unobserve(e.target); }
      });
    }, { threshold: 0.08 });
    Array.prototype.forEach.call(blocks, function (el, i) {
      el.classList.add("fade-in-up");
      el.style.transitionDelay = ((i % 5) * 60) + "ms";
      io.observe(el);
    });
  }
});
