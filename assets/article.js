// 文章统一增强：文内目录 + 代码行号 + 复制按钮 + 滚动入场动效
document.addEventListener("DOMContentLoaded", function () {
  var article = document.querySelector("article");

  // 0) 代码行号（先包行，再挂复制按钮）
  document.querySelectorAll("pre > code").forEach(function (code) {
    if (code.querySelector(".ln")) return;
    var raw = code.textContent.replace(/\n$/, "");
    var lines = raw.split("\n");
    code.textContent = "";
    lines.forEach(function (line) {
      var span = document.createElement("span");
      span.className = "ln";
      span.textContent = line.length ? line : " ";
      code.appendChild(span);
    });
  });

  // 1) 文内目录（从 h2 自动生成）
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

  // 2) 复制按钮
  document.querySelectorAll("pre").forEach(function (pre) {
    if (pre.querySelector(".copy-btn")) return;
    var btn = document.createElement("button");
    btn.className = "copy-btn";
    btn.textContent = "复制";
    btn.addEventListener("click", function () {
      var text = Array.prototype.map
        .call(pre.querySelectorAll("code .ln"), function (l) { return l.textContent; })
        .join("\n");
      if (!text) text = pre.innerText.replace(/\n$/, "");
      navigator.clipboard.writeText(text).then(function () {
        btn.textContent = "已复制";
        setTimeout(function () { btn.textContent = "复制"; }, 1500);
      });
    });
    pre.appendChild(btn);
  });

  // 3) 滚动入场动效（文章内容块，错落延迟）
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
