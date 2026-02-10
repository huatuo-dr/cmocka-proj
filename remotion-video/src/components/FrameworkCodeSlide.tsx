import { AbsoluteFill, interpolate, useCurrentFrame } from 'remotion';
import { Framework } from '../data/frameworks';

// Code example slide for each framework
export const FrameworkCodeSlide: React.FC<{
    framework: Framework;
}> = ({ framework }) => {
    const frame = useCurrentFrame();

    const opacity = interpolate(frame, [0, 20], [0, 1], { extrapolateRight: 'clamp' });
    const slideUp = interpolate(frame, [0, 20], [20, 0], { extrapolateRight: 'clamp' });

    const langColor = framework.language === 'C' ? '#EF4444' : '#3B82F6';

    return (
        <AbsoluteFill
            style={{
                backgroundColor: '#0F172A',
                color: 'white',
                fontFamily: 'Inter, sans-serif',
                opacity,
            }}
        >
            <div style={{ transform: `translateY(${slideUp}px)`, padding: '50px 60px', height: '100%', boxSizing: 'border-box' }}>
                {/* Header */}
                <div style={{ display: 'flex', alignItems: 'center', gap: 20, marginBottom: 30 }}>
                    <span style={{
                        fontSize: 48, fontWeight: 'bold',
                        background: 'linear-gradient(135deg, #60A5FA, #A78BFA)',
                        WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent',
                    }}>
                        {framework.name}
                    </span>
                    <span style={{
                        padding: '6px 16px', borderRadius: 16,
                        backgroundColor: langColor, fontSize: 18, fontWeight: 600,
                    }}>
                        {framework.language}
                    </span>
                    <span style={{ fontSize: 30, color: '#94A3B8' }}>— 代码示例</span>
                </div>

                {/* Code block */}
                <div
                    style={{
                        backgroundColor: '#1E293B',
                        borderRadius: 16,
                        padding: '30px 40px',
                        fontFamily: 'Fira Code, Consolas, monospace',
                        fontSize: 22,
                        lineHeight: 1.6,
                        whiteSpace: 'pre-wrap',
                        color: '#E2E8F0',
                        boxShadow: '0 8px 32px rgba(0,0,0,0.4)',
                        border: '1px solid #334155',
                        maxHeight: '75%',
                        overflow: 'hidden',
                    }}
                >
                    {framework.codeSnippet}
                </div>
            </div>
        </AbsoluteFill>
    );
};
