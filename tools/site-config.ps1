<#
  一次性替换全站的备案号与域名。

  用法（在仓库根目录执行）：
    pwsh tools/site-config.ps1 -Icp "浙ICP备2026000000号-1"
    pwsh tools/site-config.ps1 -Domain "redwenzi.icu"
    pwsh tools/site-config.ps1 -Icp "浙ICP备2026000000号-1" -Domain "redwenzi.icu"

  说明：
  · 备案号会带上工信部要求的链接 https://beian.miit.gov.cn/
  · 域名替换只动 https://redwenzi-blog.pages.dev（feed.xml / 文章里的绝对链接）
  · 加 -DryRun 只统计不写入
#>
param(
  [string]$Icp,
  [string]$Domain,
  [string]$Repo = (Resolve-Path "$PSScriptRoot\..").Path,
  [switch]$DryRun
)

$ErrorActionPreference = "Stop"

$skip = '\\_pagefind\\|\\\.git\\|\\node_modules\\|\\tools\\'
$files = Get-ChildItem -Path $Repo -Recurse -Include *.html,*.xml,*.md -File |
  Where-Object { $_.FullName -notmatch $skip }

$icpOld = @(
  "浙ICP备00000000号-1（备案通过后替换）",
  "浙ICP备00000000号-1"
)

$totalIcp = 0
$totalDomain = 0
$touched = @()

foreach ($f in $files) {
  $text = Get-Content -LiteralPath $f.FullName -Raw -Encoding UTF8
  $orig = $text

  if ($Icp) {
    $link = '<a href="https://beian.miit.gov.cn/" target="_blank" rel="nofollow noopener">' + $Icp + '</a>'
    foreach ($p in $icpOld) {
      $n = ([regex]::Matches($text, [regex]::Escape($p))).Count
      if ($n -gt 0) {
        # 已经被 <a> 包住的先还原，避免出现嵌套链接
        $text = $text.Replace('<a href="https://beian.miit.gov.cn/" target="_blank" rel="nofollow noopener">' + $p + '</a>', $p)
        $text = $text.Replace($p, $link)
        $totalIcp += $n
      }
    }
  }

  if ($Domain) {
    $n = ([regex]::Matches($text, [regex]::Escape("https://redwenzi-blog.pages.dev"))).Count
    if ($n -gt 0) {
      $text = $text.Replace("https://redwenzi-blog.pages.dev", "https://$Domain")
      $totalDomain += $n
    }
  }

  if ($text -ne $orig) {
    if (-not $DryRun) { Set-Content -LiteralPath $f.FullName -Value $text -Encoding UTF8 -NoNewline }
    $touched += $f.FullName.Substring($Repo.Length + 1)
  }
}

"" ; "改动文件：$($touched.Count) 个"
if ($Icp)    { "备案号替换：$totalIcp 处 → $Icp" }
if ($Domain) { "域名替换：$totalDomain 处 → https://$Domain" }
if ($DryRun) { "（DryRun：没有写入）" }
if ($touched.Count -and $touched.Count -le 12) { "" ; $touched | ForEach-Object { " · $_" } }
