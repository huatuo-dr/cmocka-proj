# Remotion Video — C/C++ 单元测试框架对比演示视频

基于 [Remotion](https://www.remotion.dev/) 框架，用 React 代码驱动生成一段 **C/C++ 单元测试框架全面对比** 的介绍视频。

## 视频参数

| 属性 | 值 |
|------|-----|
| 分辨率 | 1920 × 1080 (Full HD) |
| 帧率 | 30 fps |
| 总时长 | ~82 秒 (2460 帧) |
| 幻灯片数 | 20 页 |

## 视频内容结构

| 序号 | 内容 | 时长 |
|------|------|------|
| 1 | 封面 — "C/C++ 单元测试框架 · 8 大框架全面对比与实战指南" | 3s |
| 2 | 参赛选手一览（8 个框架卡片网格） | 3s |
| 3 | 框架对比矩阵（12 个维度 × 8 个框架） | 8s |
| 4–19 | 每个框架 2 页（概览 + 代码示例），共 8 个框架 | 各 4s |
| 20 | 结尾致谢 | 4s |

## 涵盖的框架

**C 语言**：CMocka、Unity + fff、Check

**C++**：GoogleTest + GMock、GoogleTest + MockCpp、CppUTest、Catch2、doctest

## 目录结构

```
remotion-video/
├── src/
│   ├── Root.tsx                        # Remotion 入口，定义视频参数
│   ├── Composition.tsx                 # 主合成组件，编排幻灯片时间轴
│   ├── main.tsx                        # Vite 开发入口
│   ├── InteractivePlayer.tsx           # 交互式播放器组件
│   ├── data/
│   │   └── frameworks.ts              # 8 个框架的数据与时间轴定义
│   └── components/
│       ├── ComparisonTable.tsx         # 对比矩阵表格
│       ├── FrameworkCard.tsx           # 框架概览卡片
│       └── FrameworkCodeSlide.tsx      # 框架代码展示页
├── package.json
├── vite.config.ts
└── index.html
```

## 技术栈

- **Remotion 4.0** — 用 React 编写视频
- **React 19 + TypeScript 5.9**
- **Vite 7** — 开发服务器
- **Tailwind CSS 4**

## 快速开始

```bash
# 安装依赖
npm install

# 启动 Remotion Studio 预览
npm run dev

# 渲染为视频文件
npx remotion render

# 启动 Vite 开发服务器（交互式播放器）
npm start
```
