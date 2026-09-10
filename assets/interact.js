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
