# Remotion 交互式演示大纲：C/C++ 单元测试框架对比

## 1. 核心交互逻辑 (Option A: Player Integration)
- **技术选型**: `@remotion/player` + 手动控制
- **实现方式**:
    - **视频**: 作为一个整体的连续视频。
    - **控制层**: 一个 React 组件包裹着 `<Player>`，并提供 "Previous", "Next" 按钮。
    - **时间轴映射**: 定义一个 `slideTimestamps` 数组，例如 `[0, 150, 450, 900]` (帧数)。
    - **交互**: 点击 "Next" -> `playerRef.current.seekTo(slideTimestamps[currentSlide + 1])` 并自动播放过渡动画，然后暂停。

## 2. 场景脚本 & 幻灯片划分 (Slides)

所有场景默认处于 **暂停 (PAUSED)** 状态，等待演讲者讲解。点击 "Next" 触发转场动画进入下一页。

### Slide 1: 封面 (The Hook)
- **静止态**: 标题 "C/C++ Unit Testing Frameworks" 缓缓浮动。
- **转场动作**: 标题文字上移消失，背景粒子加速。
- **关键帧**: 0 ~ 150 (5秒)

### Slide 2: 选手登场 (The Contenders)
- **静止态**: 8 个框架 Logo 网格状排列。
- **交互**: (可选) 鼠标悬停在 Logo 上时，Logo 放大 (需 Remotion Player 额外支持 DOM 覆盖层，或简化为视频内动画)。
- **转场动作**: 网格变身为表格的列头。
- **关键帧**: 150 ~ 390 (8秒)

### Slide 3: 核心对比概览 (The Matrix)
- **静止态**: 完整对比表格。
- **重点**: 高亮 "C语言" 和 "Mock机制" 两列。
- **转场动作**: 表格淡出，屏幕一分为二 (左侧代码，右侧特性)。
- **关键帧**: 390 ~ 840 (15秒)

### Slide 4~11: 框架详情 (Framework Deep Dives)
*(每页结构一致，点击 Next 切换到下一个框架)*

- **Slide 4: CMocka**
    - 内容: C语言 | 链接期 Mock (--wrap)
    - 关键帧: 840 ~ 1140 (10秒)
- **Slide 5: Unity + fff**
    - 内容: 嵌入式首选 | 函数指针 Mock
    - 关键帧: 1140 ~ 1440 (10秒)
- **Slide 6: GoogleTest**
    - 内容: C++ 标准 | 强大的 Matchers
    - 关键帧: 1440 ~ 1740 (10秒)
- **Slide 7: CppUTest**
    - 内容: 内存泄漏检测 | C/C++ 混合
    - 关键帧: 1740 ~ 2040 (10秒)
- **Slide 8: Check**
    - 内容: 多进程隔离 (Fork) | 信号测试
    - 关键帧: 2040 ~ 2340 (10秒)
- **Slide 9: Catch2 / doctest**
    - 内容: Header-only | BDD 风格 | 极速编译
    - 关键帧: 2340 ~ 2640 (10秒)

### Slide 12: 总结与资源 (Outro)
- **静止态**: GitHub 二维码 + Star 引导。
- **循环动画**: 底部 "Thank You" 文字呼吸效果。
- **关键帧**: 2640 ~ End

## 3. 技术实现提示

### 数据结构 (Data Driven)
```typescript
export const SLIDES = [
  { id: 'intro', durationInFrames: 150, component: IntroSlide },
  { id: 'contenders', durationInFrames: 240, component: ContendersList },
  { id: 'matrix', durationInFrames: 450, component: ComparisonTable },
  // ... frameworks
];
```

### 播放器控制 (Player Wrapper)
我们需要创建一个 `InteractivePlayer.tsx`：
- `currentSlideIndex` (State)
- `nextSlide()`: 计算下一张幻灯片的起始帧，执行 `player.seekTo()`。
- `Capture Controls`: 监听键盘 `RightArrow` / `Space` 键。
