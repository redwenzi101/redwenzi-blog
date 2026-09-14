# redwenzi 的博客

一个基于纯 HTML/CSS 的静态博客首页（备案完成后可作为博客原型）。

## 文件

- `index.html`：博客主页（可本地预览，双击打开）
- `knowledge/index.html`：📚 知识大全（工具指南，持续补充）
- `posts/build-blog-guide.html`：《博客搭建指南：从域名到上线》文章页
- `posts/what-is-ai.html`：《AI 是什么？它能做什么？》文章页
- `posts/ai-agent-guide.html`：《AI Agent 上手实操：从调 API 到最小 Agent》文章页
- `posts/ways-to-use-ai.html`：《用 AI 的几种方式：App / 命令行 / API / 中转站》文章页
- `posts/web-domain-server-guide.html`：《网站、网址、域名、服务器》科普文章页
- `programming/index.html`：💻 编程语言分享（C / C++ / Java / Python 学习路线）
- `programming/cpp/index.html`：《C++ 新手进阶教材》目录页（13 章 · 98 道练习 · 四周计划）
- `programming/cpp/01-…13-*.html`：C++ 教材 13 章正文（含练习与折叠答案）
- `programming/cpp/problems.html`：全部 98 道练习的索引页
- `assets/course.css`：课程页面组件样式（练习卡片 / 样例块 / 示意图，多语言共用）

## 内容结构（统筹）

- 🏗️ 建站与运维线：网站/域名/服务器科普 → 博客搭建指南
- 🤖 AI 与大模型线：AI 是什么 → 用 AI 的几种方式 → AI Agent 上手实操
- 💻 编程语言分享线：C（规划中）→ C++（已上线，13 章 98 题）→ Java（规划中）→ Python（规划中）
- 📚 知识大全：所有文章中的概念词条，按锚点供文章跳转，随时补充

## C++ 教材的维护约定

- 正文页由 `work/build/` 下的生成脚本产出（解析原 docx → 结构化 JSON → 生成 HTML），改内容优先改脚本再重新生成，避免手工改 13 个页面。
- 每章练习的编号规则：`小节号 + 该小节内序号`，例如 `4.2.1`；`?A/?B` 结尾的是本章综合练习。
- 所有 C++ 代码块要求能用 `g++ -std=c++17 -Wall -Wextra -Wpedantic` 零警告编译；新增或修改代码后要重跑校验脚本。

## 文章与知识库联动规则

- 每篇文章文末都有「📚 本文知识点」链接块
- 点击词条跳转到 `knowledge/index.html#词条id`
- 新文章发布时，把陌生概念补进知识大全并在文中挂链接
- 每次发文章都要同步更新知识大全（见 AGENTS.md）

## 预览

直接用浏览器打开 `index.html` 即可。

## 后续

备案通过后，可将其部署到服务器 `/var/www/example`，或作为 WordPress 的上线参考。
