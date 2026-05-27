# SDR++ 汉化版

[![License: GPL-3.0](https://img.shields.io/badge/License-GPL%203.0-blue.svg)](license)
[![上游同步](https://github.com/AlexandreRouma/SDRPlusPlus/workflows/Build%20Binaries/badge.svg)](https://github.com/AlexandreRouma/SDRPlusPlus)

> **衍生作品声明**：本仓库是 [AlexandreRouma/SDRPlusPlus](https://github.com/AlexandreRouma/SDRPlusPlus) 的修改版本。
> 原作品版权 © AlexandreRouma，以 GNU General Public License v3.0 发布。
> 本修改版本同样以 GPL-3.0 发布，修改内容为界面中文化、CJK 字体适配及自动化构建流程。
> 修改日期：2025 年 5 月。

基于 SDR++ 的中文汉化版本，实现**自动拉取上游更新 → 自动汉化 → 自动编译输出**的完整流程。

## 与上游的主要差异

| 修改类别 | 内容 |
|---------|------|
| 界面汉化 | 81 个源文件中的英文 UI 字符串替换为中文 |
| CJK 字体 | 自动检测系统 CJK 字体，baseFont / bigFont 均合并 `ChineseFull` 字形范围 |
| 编译适配 | MSVC 添加 `/utf-8` 标志，确保 UTF-8 字符串字面量正确编译 |
| CI/CD | 新增上游同步检测、汉化构建、多平台编译发布工作流 |
| 自动化脚本 | `scripts/` 目录新增同步与汉化流程脚本 |

## 汉化范围

| 类别 | 模块 | 状态 |
|------|------|------|
| 核心界面 | 主窗口、信号源菜单、显示设置、频段规划、主题、VFO 颜色、模块管理、关于对话框、瀑布图控件 | ✅ |
| 解调模块 | Radio (AM/NFM/WFM/DSB/USB/CW/LSB)、寻呼机、气象卫星、Falcon9、ATV、VOR、Meteor、M17、SSTV | ✅ |
| 杂项模块 | 录制器、频率管理器、rigctl 服务器/客户端、扫描器、IQ 导出、调度器 | ✅ |
| 输出模块 | 音频输出、网络音频输出 | ✅ |
| 信号源模块 | 全部 28+ 信号源模块 | ✅ |

### 汉化约定

- **技术缩写保留英文**：AGC, LNA, VGA, Bias-T, FM, PPM, IQ, RF, VFO, FFT, SNR, CTCSS, RDS, DCS 等
- **单位保留英文**：MHz, kHz, Hz
- **调制模式保留英文**：NFM, WFM, AM, DSB, USB, CW, LSB, RAW
- **ImGui 内部 ID**（`##xxx` 后缀）保持不变
- **OptionList.define()** 第一个参数（key）不变，只改第二个参数（显示文本）
- **不翻译**：ImGui 库文件、主题/色彩图/频段规划 JSON 名称、开发者日志消息

### CJK 字体适配

| 平台 | 检测字体 | 字形范围 |
|------|---------|---------|
| Windows | `msyh.ttc`（微软雅黑） | `GetGlyphRangesChineseFull()` |
| macOS | `PingFang.ttc`（苹方） | `GetGlyphRangesChineseFull()` |
| Android | `NotoSansCJK-Regular.ttc` / `NotoSansSC-Regular.otf` | `GetGlyphRangesChineseFull()` |
| Linux | `NotoSansCJK*.ttc` / `WenQuanYi*.ttc` | `GetGlyphRangesChineseFull()` |

- baseFont（默认 UI 字体）与 bigFont（频率窗口 45px）均已合并 CJK 支持
- MSVC 编译添加 `/utf-8` 标志，防止 GBK 误编码

## 自动化流程

```
上游新提交 → GitHub Actions 检测 → 合并到本仓库 → AI 汉化补丁 → 自动编译 → Release 发布
```

### 上游同步

```bash
# 检查上游是否有新提交
./scripts/sync_upstream.sh check

# 合并上游更新
./scripts/sync_upstream.sh merge
```

- **GitHub Actions**（`.github/workflows/sync-upstream.yml`）：每日定时检查上游更新
- **汉化文件清单**（`scripts/localized_files.txt`）：记录 81 个已汉化文件路径

### CI/CD 工作流

| 工作流 | 文件 | 功能 |
|--------|------|------|
| 上游同步 | `sync-upstream.yml` | 每日检测上游新提交 |
| 汉化构建+发布 | `build_cn.yml` | 编译 Windows + Android 版本，自动创建 GitHub Release |
| 全平台构建 | `build_all.yml` | 多平台编译发布 |

### 下载

每次推送到 `master` 分支后自动构建并发布到 [Releases](../../releases) 页面。

| 文件 | 平台 | 说明 |
|------|------|------|
| `sdrpp_windows_x64_cn.zip` | Windows x64 | 解压后运行 `sdrpp.exe` |
| `sdrpp_android_cn.apk` | Android | 侧载安装，需要 RTL-SDR 等 USB SDR 硬件 |

Release 标签格式：`cn-YYYY.MM.DD`（同日多次推送追加短 SHA）。自动保留最近 10 个 Release。

## 仓库结构

```
SDRPlusPlus_CN/
├── scripts/                    # 自动化流程脚本
│   ├── sync_upstream.sh        #   上游同步脚本
│   └── localized_files.txt     #   已汉化文件清单
├── .github/workflows/          # CI/CD 工作流
├── .workbuddy/                 # AI 汉化自动化配置
├── CMakeLists.txt              # 构建配置（含 /utf-8 标志）
├── core/                       # 核心界面源码
├── decoder_modules/            # 解调模块源码
├── misc_modules/               # 杂项模块源码
├── sink_modules/               # 输出模块源码
├── source_modules/             # 信号源模块源码
├── root/                       # 资源文件（主题/色彩图/图标）
├── src/                        # 主入口源码
└── license                     # GPL-3.0 许可证
```

> 源码目录结构与上游保持一致，确保 `git merge` 同步时不产生路径冲突。

## AI 辅助开发

本项目的汉化工作由 [WorkBuddy](https://workbuddy.ai) AI 辅助编程工具完成。WorkBuddy 在以下环节发挥了关键作用：

| 环节 | 说明 |
|------|------|
| 界面汉化 | 批量识别 81 个源文件中的英文 UI 字符串并替换为中文，自动遵循汉化约定（保留 ImGui ID、技术缩写等） |
| CJK 字体适配 | 定位 ImGui 字体系统，实现跨平台 CJK 字体检测与合并，修复中文字符显示问号/方框问题 |
| 编译问题排查 | 诊断 MSVC UTF-8 编码问题，添加 `/utf-8` 编译标志 |
| 自动化流程搭建 | 编写上游同步脚本、GitHub Actions 工作流、WorkBuddy 定时自动化任务 |
| 增量更新支持 | 合并上游更新后，基于 `localized_files.txt` 清单自动识别冲突并重新汉化 |

`.workbuddy/` 目录包含项目级别的 AI 工作配置和上下文记忆，用于在后续上游同步时自动恢复汉化上下文。

## 编译

### Windows (MSVC)

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

### Linux

```bash
mkdir build && cd build
cmake .. -DOPT_OVERRIDE_STD_FILESYSTEM=ON
make -j$(nproc)
```

### 依赖

参考上游 [SDR++ 编译指南](https://github.com/AlexandreRouma/SDRPlusPlus#compiling)。

## 相关链接

- **上游仓库**：[AlexandreRouma/SDRPlusPlus](https://github.com/AlexandreRouma/SDRPlusPlus)
- **原作者 Patreon**：[patreon.com/ryzerth](https://patreon.com/ryzerth)
- **原作者 Discord**：[discord.gg/aFgWjyD](https://discord.gg/aFgWjyD)
- **原版下载**：[sdrpp.org/nightly](https://www.sdrpp.org/nightly)

## License

本仓库以 [GNU General Public License v3.0](license) 发布，与上游许可证一致。
