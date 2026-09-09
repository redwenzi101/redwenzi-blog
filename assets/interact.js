// 互动组件：回到顶部 + 每篇文章点赞（仅本站演示，localStorage）
document.addEventListener("DOMContentLoaded", function () {
  // 回到顶部
  var top = document.createElement("button");
  top.className = "backtop";
  top.textContent = "↑";
  top.title = "回到顶部";
  top.addEventListener("click", function () { window.scrollTo({ top: 0, behavior: "smooth" }); });
  document.body.appendChild(top);
  window.addEventListener("scroll", function () {
    top.style.display = window.scrollY > 300 ? "flex" : "none";
  }, { passive: true });

  // 点赞（单站演示：记录本机，跨用户不作数）
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
