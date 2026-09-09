# redwenzi 的博客

一个基于纯 HTML/CSS 的静态博客首页（备案完成后可作为博客原型）。

## 文件

- `index.html`：博客主页（可本地预览，双击打开）
- `knowledge/index.html`：📚 知识大全（工具指南，持续补充）
- `posts/build-blog-guide.html`：《博客搭建指南：从域名到上线》文章页
- `posts/ai-agent-guide.html`：《AI Agent 入门指南：Agent、API 与 Token》文章页
- `posts/ways-to-use-ai.html`：《用 AI 的几种方式：App / 命令行 / API / 中转站》文章页
- `posts/web-domain-server-guide.html`：《网站、网址、域名、服务器》科普文章页

## 内容结构（统筹）

- 🏗️ 建站与运维：博客搭建指南、网站/域名/服务器科普
- 🤖 AI 与大模型：AI Agent 上手实操、用 AI 的几种方式
- 📚 知识大全：所有文章中的概念词条，按锚点供文章跳转，随时补充

## 文章与知识库联动规则

- 每篇文章文末都有「📚 本文知识点」链接块
- 点击词条跳转到 `knowledge/index.html#词条id`
- 新文章发布时，把陌生概念补进知识大全并在文中挂链接

## 预览

直接用浏览器打开 `index.html` 即可。

## 后续

备案通过后，可将其部署到服务器 `/var/www/example`，或作为 WordPress 的上线参考。
