import { AbsoluteFill, Sequence, interpolate, useCurrentFrame } from 'remotion';
import { ComparisonTable } from './components/ComparisonTable';
import { FrameworkCard } from './components/FrameworkCard';
import { FrameworkCodeSlide } from './components/FrameworkCodeSlide';
import { FRAMEWORKS, SLIDE_TIMESTAMPS } from './data/frameworks';

// Helper to compute slide duration
const slideDuration = (index: number) => {
  const next = SLIDE_TIMESTAMPS[index + 1] ?? SLIDE_TIMESTAMPS[index] + 120;
  return next - SLIDE_TIMESTAMPS[index];
};

// Intro
const IntroSequence: React.FC = () => {
  const frame = useCurrentFrame();
  const opacity = interpolate(frame, [0, 20], [0, 1], { extrapolateRight: 'clamp' });
  const scale = interpolate(frame, [0, 20], [0.9, 1], { extrapolateRight: 'clamp' });

  return (
    <AbsoluteFill style={{
      backgroundColor: '#0F172A', justifyContent: 'center', alignItems: 'center',
      fontFamily: 'Inter, sans-serif', opacity,
    }}>
      <div style={{ transform: `scale(${scale})`, textAlign: 'center' }}>
        <h1 style={{ fontSize: 90, margin: 0, background: 'linear-gradient(135deg, #60A5FA, #A78BFA, #F472B6)', WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent' }}>
          C/C++ 单元测试框架
        </h1>
        <h2 style={{ fontSize: 44, color: '#94A3B8', marginTop: 20 }}>
          8 大框架全面对比与实战指南
        </h2>
      </div>
    </AbsoluteFill>
  );
};

// Contenders grid
const ContendersSlide: React.FC = () => {
  const frame = useCurrentFrame();
  const opacity = interpolate(frame, [0, 15], [0, 1], { extrapolateRight: 'clamp' });

  return (
    <AbsoluteFill style={{
      backgroundColor: '#0F172A', justifyContent: 'center', alignItems: 'center',
      fontFamily: 'Inter, sans-serif', opacity,
    }}>
      <h1 style={{ fontSize: 48, color: '#93C5FD', marginBottom: 40 }}>🏆 参赛选手</h1>
      <div style={{ display: 'grid', gridTemplateColumns: 'repeat(4, 1fr)', gap: 30, width: '80%' }}>
        {FRAMEWORKS.map((f, i) => {
          const delay = i * 3;
          const itemOpacity = interpolate(frame, [10 + delay, 20 + delay], [0, 1], { extrapolateRight: 'clamp' });
          const langColor = f.language === 'C' ? '#EF4444' : '#3B82F6';
          return (
            <div key={f.id} style={{
              backgroundColor: '#1E293B', borderRadius: 16, padding: '24px 20px',
              textAlign: 'center', opacity: itemOpacity,
              border: `2px solid ${langColor}30`,
            }}>
              <div style={{ fontSize: 28, fontWeight: 'bold', color: 'white', marginBottom: 8 }}>
                {f.name}
              </div>
              <div style={{ display: 'flex', justifyContent: 'center', gap: 10 }}>
                <span style={{ backgroundColor: langColor, borderRadius: 10, padding: '3px 10px', fontSize: 14, color: 'white' }}>
                  {f.language}
                </span>
                <span style={{ backgroundColor: '#334155', borderRadius: 10, padding: '3px 10px', fontSize: 14, color: '#94A3B8' }}>
                  {f.mockType}
                </span>
              </div>
            </div>
          );
        })}
      </div>
    </AbsoluteFill>
  );
};

// Outro
const OutroSequence: React.FC = () => {
  const frame = useCurrentFrame();
  const opacity = interpolate(frame, [0, 20], [0, 1], { extrapolateRight: 'clamp' });

  return (
    <AbsoluteFill style={{
      backgroundColor: '#0F172A', justifyContent: 'center', alignItems: 'center',
      fontFamily: 'Inter, sans-serif', opacity,
    }}>
      <h1 style={{ fontSize: 72, color: 'white', textAlign: 'center' }}>
        🎉 感谢观看
      </h1>
      <h2 style={{ fontSize: 36, color: '#94A3B8', marginTop: 20, textAlign: 'center' }}>
        完整代码 & 文档
      </h2>
      <p style={{ fontSize: 32, color: '#60A5FA', marginTop: 10 }}>
        github.com/huatuo/clang-ut-tools
      </p>
      <p style={{ fontSize: 24, color: '#64748B', marginTop: 30 }}>
        ⭐ Star & Fork — 一起学习单元测试!
      </p>
    </AbsoluteFill>
  );
};

export const Composition: React.FC = () => {
  return (
    <AbsoluteFill style={{ backgroundColor: '#0F172A' }}>
      {/* 1. Intro */}
      <Sequence from={SLIDE_TIMESTAMPS[0]} durationInFrames={slideDuration(0)}>
        <IntroSequence />
      </Sequence>

      {/* 2. Contenders */}
      <Sequence from={SLIDE_TIMESTAMPS[1]} durationInFrames={slideDuration(1)}>
        <ContendersSlide />
      </Sequence>

      {/* 3. Comparison matrix (single page) */}
      <Sequence from={SLIDE_TIMESTAMPS[2]} durationInFrames={slideDuration(2)}>
        <ComparisonTable />
      </Sequence>

      {/* 4-19: Frameworks (2 slides each: overview + code) */}
      {FRAMEWORKS.map((framework, index) => {
        const overviewIdx = 3 + index * 2;   // slide index for overview
        const codeIdx = 3 + index * 2 + 1;   // slide index for code

        return (
          <div key={framework.id}>
            <Sequence from={SLIDE_TIMESTAMPS[overviewIdx]} durationInFrames={slideDuration(overviewIdx)}>
              <FrameworkCard framework={framework} />
            </Sequence>
            <Sequence from={SLIDE_TIMESTAMPS[codeIdx]} durationInFrames={slideDuration(codeIdx)}>
              <FrameworkCodeSlide framework={framework} />
            </Sequence>
          </div>
        );
      })}

      {/* 21. Outro */}
      <Sequence from={SLIDE_TIMESTAMPS[SLIDE_TIMESTAMPS.length - 1]} durationInFrames={120}>
        <OutroSequence />
      </Sequence>
    </AbsoluteFill>
  );
};
