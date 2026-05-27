# SDR++ 汉化项目进度

## 项目信息
- 仓库: E:\GIT\SDRPlusPlus_CN
- 任务: SDR++ 界面全面中文化
- 策略: 直接替换 C++ 源文件中的英文字符串字面量，保留 ImGui ID（##xxx 后缀）
- 状态: **全部完成** ✅

## 已完成模块

### core/ (核心界面)
- `core/src/gui/main_window.cpp` — 菜单注册、调试面板、加载屏幕、瀑布图控件
- `core/src/gui/menus/source.cpp` — 信号源菜单（IQ校正、偏移量、抽取）
- `core/src/gui/menus/display.cpp` — 显示设置菜单
- `core/src/gui/menus/bandplan.cpp` — 频段规划菜单
- `core/src/gui/menus/theme.cpp` — 主题菜单
- `core/src/gui/menus/vfo_color.cpp` — VFO颜色菜单
- `core/src/gui/menus/module_manager.cpp` — 模块管理器菜单
- `core/src/gui/dialogs/credits.cpp` — 关于/致谢对话框
- `core/src/gui/widgets/waterfall.cpp` — 瀑布图 VFO 信号强度提示、Ctrl+点击频率/带宽/带宽锁定 tooltip

### decoder_modules/ (解调模块)
- `decoder_modules/radio/src/radio_module.h` — Radio 模块主 UI
- `decoder_modules/radio/src/demodulators/am.h` — AM 解调器
- `decoder_modules/radio/src/demodulators/cw.h` — CW 解调器
- `decoder_modules/radio/src/demodulators/dsb.h` — DSB 解调器
- `decoder_modules/radio/src/demodulators/lsb.h` — LSB 解调器
- `decoder_modules/radio/src/demodulators/usb.h` — USB 解调器
- `decoder_modules/radio/src/demodulators/wfm.h` — WFM 解调器
- `decoder_modules/radio/src/demodulators/nfm.h` — NFM 解调器
- `decoder_modules/pager_decoder/` — 寻呼机解码器（协议、录制、查看消息）
- `decoder_modules/weather_sat_decoder/` — 气象卫星解码器（录制、显示图像）
- `decoder_modules/falcon9_decoder/` — Falcon9 遥测解码器（隐藏/显示日志、清除日志）
- `decoder_modules/atv_decoder/` — ATV 解码器（水平/垂直同步、快速锁定、彩色模式、增益/偏移/副载波信息）
- `decoder_modules/vor_receiver/` — VOR 接收器（方位角、信号质量）
- `decoder_modules/meteor_demodulator/` — Meteor 解调器（录制/停止、录制状态）
- `decoder_modules/m17_decoder/` — M17 解码器（源/目标/数据类型/加密信息、参考线、状态）
- `decoder_modules/m17_decoder/src/lsf_decode.cpp` — M17 数据类型和加密类型文本

### misc_modules/ (杂项模块)
- `misc_modules/recorder/src/main.cpp` — 录制器模块
- `misc_modules/frequency_manager/src/main.cpp` — 频率管理器模块
- `misc_modules/rigctl_server/src/main.cpp` — rigctl 服务器模块
- `misc_modules/rigctl_client/src/main.cpp` — rigctl 客户端模块
- `misc_modules/scanner/src/main.cpp` — 扫描器模块
- `misc_modules/iq_exporter/src/main.cpp` — IQ 导出模块（含错误提示框）
- `misc_modules/scheduler/src/sched_task.h` — 调度器任务
- `misc_modules/scheduler/src/actions/tune_vfo.h` — 调度器 VFO 调谐动作

### sink_modules/ (输出模块)
- `sink_modules/network_sink/src/main.cpp` — 网络音频输出

### source_modules/ (信号源模块) — 全部 28+ 模块
- rtl_sdr_source, soapy_source, hackrf_source, airspy_source, airspyhf_source
- sdrplay_source, plutosdr_source, rtl_tcp_source, network_source, sdrpp_server_source
- spyserver_source, rfspace_source, spectran_source, spectran_http_source
- bladerf_source, limesdr_source, usrp_source, file_source, audio_source
- harogic_source, hydrasdr_source, fobossdr_source, dragonlabs_source
- kcsdr_source, perseus_source, hermes_source, sddc_source, rfnm_source

## 汉化约定
- 技术缩写保留英文: AGC, LNA, VGA, Bias-T, FM, DAB, MW, PPM, IQ, RF, IF, HF, VHF, USB, UDP, TCP, RTL, SDR, VFO, FFT, SNR, CTCSS, RDS, DCS, NOAA, APT 等
- 采样率单位保留英文: MHz, KHz, kHz, Hz
- 调制模式缩写保留: NFM, WFM, AM, DSB, USB, CW, LSB, RAW
- ImGui 内部 ID（##xxx 后缀）必须保持不变
- OptionList.define() 的第一个参数（key）保持不变，只改第二个参数（显示文本）
- 保留名称的比较逻辑需同步更新（如 offsets.define("无", ...) 后 strcmp 也改为 "无"）
- imgui_demo.cpp 是 ImGui 库的演示文件，不需要翻译
- MSVC 编译必须加 /utf-8（CMakeLists.txt 已配置），否则中文字符串按 GBK 解读致乱码
- 中文字体通过系统 CJK 字体 + ImGui MergeMode 合并到 Roboto baseFont（style.cpp 已配置）
- 使用 GetGlyphRangesChineseFull() 完整中文字形范围（非 ChineseSimplifiedCommon）
- bigFont（45px 频率窗口字体）也已合并 CJK 字体，支持中文标签
- Sink provider 注册名已翻译：Audio→音频, Network→网络, New Audio→新音频
- defConfig 菜单/模块实例名已翻译：Source→信号源, Recorder→录制器, Frequency Manager→频率管理器 等

## 不翻译的内容
- imgui_demo.cpp / imgui_widgets.cpp 等 ImGui 库文件
- 主题名称 JSON（Light, Dark, Deep Blue 等是设计专有名词）
- 色彩图名称 JSON（Classic, Inferno, Viridis 等是技术名）
- bandplan 名称 JSON（业余无线电国际标准术语）
- flog::error / printf 等开发者日志消息
