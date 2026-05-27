# SDR++ 汉化版 — 自动化构建仓库

[![上游同步](https://github.com/AlexandreRouma/SDRPlusPlus/workflows/Build%20Binaries/badge.svg)](https://github.com/AlexandreRouma/SDRPlusPlus)

基于 [AlexandreRouma/SDRPlusPlus](https://github.com/AlexandreRouma/SDRPlusPlus) 的 SDR++ 中文汉化版本，实现了**自动拉取上游更新 → 自动汉化 → 自动编译输出**的完整自动化流程。

## 仓库结构

```
SDRPlusPlus_CN/
├── scripts/                    # 🔄 自动化流程脚本
│   ├── sync_upstream.sh        # 上游同步脚本（check/merge 模式）
│   └── localized_files.txt     # 已汉化文件清单（67 个文件）
├── .github/                    # ⚙️ CI/CD 工作流
│   └── workflows/
│       └── sync-upstream.yml   # 每日自动检测上游更新
├── .workbuddy/                 # 🤖 AI 汉化自动化配置
│
├── CMakeLists.txt              # 构建配置（已添加 MSVC /utf-8 标志）
├── core/                       # 核心界面源码（已汉化）
├── decoder_modules/            # 解调模块源码（已汉化）
├── misc_modules/               # 杂项模块源码（已汉化）
├── sink_modules/               # 输出模块源码（已汉化）
├── source_modules/             # 信号源模块源码（已汉化）
├── root/                       # 资源文件（主题/色彩图/图标等）
├── src/                        # 主入口源码
├── android/                    # Android 构建配置
├── docker_builds/              # Docker 构建脚本
├── macos/                      # macOS 构建脚本
├── win32/                      # Windows 构建脚本
└── readme.md                   # 本文件
```

> **注意**：源码目录保持与上游一致的文件结构，以确保 `git merge` 同步更新时不会产生路径冲突。

## 自动化流程

### 1. 上游同步检测

GitHub Actions 每日定时检查上游仓库是否有新提交：

```bash
# 手动检查上游更新
./scripts/sync_upstream.sh check

# 合并上游更新
./scripts/sync_upstream.sh merge
```

### 2. 自动汉化

当检测到上游更新并合并后，WorkBuddy 自动化流程将：
1. 对比 `scripts/localized_files.txt` 中的文件列表
2. 识别合并冲突或新增内容
3. 重新执行汉化补丁

### 3. 自动编译

通过 GitHub Actions 完成多平台编译输出。

## 汉化范围

### 已完成模块

| 类别 | 模块 | 状态 |
|------|------|------|
| 核心界面 | 主窗口、信号源菜单、显示设置、频段规划、主题、VFO 颜色、模块管理、关于对话框、瀑布图控件 | ✅ |
| 解调模块 | Radio (AM/NFM/WFM/DSB/USB/CW/LSB)、寻呼机、气象卫星、Falcon9、ATV、VOR、Meteor、M17 | ✅ |
| 杂项模块 | 录制器、频率管理器、rigctl 服务器/客户端、扫描器、IQ 导出、调度器 | ✅ |
| 输出模块 | 网络音频输出 | ✅ |
| 信号源模块 | 全部 28+ 信号源模块 | ✅ |

### 汉化约定

- **技术缩写保留英文**：AGC, LNA, VGA, Bias-T, FM, DAB, PPM, IQ, RF, VFO, FFT, SNR, CTCSS, RDS 等
- **单位保留英文**：MHz, kHz, Hz
- **调制模式保留英文**：NFM, WFM, AM, DSB, USB, CW, LSB, RAW
- **ImGui 内部 ID**（`##xxx` 后缀）保持不变
- **OptionList.define()** 第一个参数（key）保持不变，只改第二个参数（显示文本）
- **不翻译的内容**：ImGui 库文件、主题名称 JSON、色彩图名称 JSON、频段规划名称 JSON、开发者日志消息

### CJK 字体支持

- 自动检测系统 CJK 字体（Windows: 微软雅黑, macOS: 苹方, Linux: NotoSansCJK）
- 使用 `GetGlyphRangesChineseFull()` 完整中文字形范围
- baseFont、bigFont 均已合并 CJK 字体支持
- MSVC 编译器已添加 `/utf-8` 标志

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
- **Patreon**：[patreon.com/ryzerth](https://patreon.com/ryzerth)
- **Discord**：[discord.gg/aFgWjyD](https://discord.gg/aFgWjyD)
- **原版下载**：[sdrpp.org/nightly](https://www.sdrpp.org/nightly)

## License

与上游相同，遵循 [GPLv3 License](license)。
