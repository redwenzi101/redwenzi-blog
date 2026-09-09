// 为所有文章统一增强：自动生成文内目录 + 代码块复制按钮
document.addEventListener("DOMContentLoaded", function () {
  var article = document.querySelector("article");

  // 1) 从 h2 自动生成「本文目录」，插到文章开头
  if (article) {
    var heads = Array.prototype.slice.call(article.querySelectorAll("h2"));
    if (heads.length) {
      var box = document.createElement("div");
      box.className = "toc-inline";
      box.innerHTML = "<b>📖 本文目录</b><ol></ol>";
      var list = box.querySelector("ol");
      var slug = "sec";
      heads.forEach(function (h, i) {
        var id = slug + "-" + (i + 1);
        if (!h.id) h.id = id;
        var li = document.createElement("li");
        var a = document.createElement("a");
        a.href = "#" + h.id;
        a.textContent = h.textContent;
        li.appendChild(a);
        list.appendChild(li);
      });
      article.insertBefore(box, article.firstChild);
      // 让 h2 有 id 便于滚动定位
      heads.forEach(function (h) { if (!h.id) h.id = ""; });
    }
  }

  // 2) 给每个代码块加复制按钮
  document.querySelectorAll("pre").forEach(function (pre) {
    if (pre.querySelector(".copy-btn")) return;
    var btn = document.createElement("button");
    btn.className = "copy-btn";
    btn.textContent = "复制";
    btn.addEventListener("click", function () {
      var text = pre.innerText.replace(/\n$/, "");
      navigator.clipboard.writeText(text).then(function () {
        btn.textContent = "已复制";
        setTimeout(function () { btn.textContent = "复制"; }, 1500);
      });
    });
    pre.appendChild(btn);
  });
});
