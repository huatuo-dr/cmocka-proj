import { AbsoluteFill, interpolate, useCurrentFrame } from 'remotion';
import { Framework } from '../data/frameworks';

// Overview slide: name, description, install, features, pros/cons, best for
export const FrameworkCard: React.FC<{
    framework: Framework;
}> = ({ framework }) => {
    const frame = useCurrentFrame();

    const opacity = interpolate(frame, [0, 20], [0, 1], { extrapolateRight: 'clamp' });
    const slideUp = interpolate(frame, [0, 20], [30, 0], { extrapolateRight: 'clamp' });

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
                <div style={{ display: 'flex', alignItems: 'center', gap: 20, marginBottom: 10 }}>
                    <span style={{
                        fontSize: 64, fontWeight: 'bold',
                        background: 'linear-gradient(135deg, #60A5FA, #A78BFA)',
                        WebkitBackgroundClip: 'text', WebkitTextFillColor: 'transparent',
                    }}>
                        {framework.name}
                    </span>
                    <span style={{
                        padding: '6px 16px', borderRadius: 16,
                        backgroundColor: langColor, fontSize: 20, fontWeight: 600,
                    }}>
                        {framework.language}
                    </span>
                    <span style={{ padding: '6px 16px', borderRadius: 16, backgroundColor: '#334155', fontSize: 18 }}>
                        Mock: {framework.mockType}
                    </span>
                </div>

                {/* Description */}
                <p style={{ fontSize: 28, color: '#94A3B8', margin: '0 0 30px 0' }}>
                    {framework.description}
                </p>

                {/* Two columns */}
                <div style={{ display: 'grid', gridTemplateColumns: '1fr 1fr', gap: 40 }}>
                    {/* Left: Install + Features */}
                    <div>
                        {/* Install */}
                        <h3 style={{ color: '#60A5FA', fontSize: 24, margin: '0 0 10px 0' }}>📦 安装</h3>
                        <div style={{
                            backgroundColor: '#1E293B', borderRadius: 10, padding: 16,
                            fontFamily: 'Fira Code, monospace', fontSize: 16,
                            whiteSpace: 'pre-wrap', marginBottom: 24, color: '#A5F3FC',
                        }}>
                            {framework.installCmd}
                        </div>

                        {/* Features */}
                        <h3 style={{ color: '#60A5FA', fontSize: 24, margin: '0 0 10px 0' }}>✨ 核心特性</h3>
                        <ul style={{ listStyle: 'none', padding: 0, margin: 0 }}>
                            {framework.features.map((f, i) => (
                                <li key={i} style={{ fontSize: 22, marginBottom: 8, display: 'flex', alignItems: 'center', gap: 10 }}>
                                    <span style={{ color: '#60A5FA' }}>▸</span> {f}
                                </li>
                            ))}
                        </ul>
                    </div>

                    {/* Right: Pros, Cons, Best For */}
                    <div>
                        <h3 style={{ color: '#4ADE80', fontSize: 24, margin: '0 0 10px 0' }}>✅ 优势</h3>
                        <ul style={{ listStyle: 'none', padding: 0, margin: '0 0 20px 0' }}>
                            {framework.pros.map((p, i) => (
                                <li key={i} style={{ fontSize: 22, marginBottom: 8, display: 'flex', alignItems: 'center', gap: 10 }}>
                                    <span style={{ color: '#4ADE80' }}>✓</span> {p}
                                </li>
                            ))}
                        </ul>

                        <h3 style={{ color: '#F87171', fontSize: 24, margin: '0 0 10px 0' }}>⚠️ 不足</h3>
                        <ul style={{ listStyle: 'none', padding: 0, margin: '0 0 20px 0' }}>
                            {framework.cons.map((c, i) => (
                                <li key={i} style={{ fontSize: 22, marginBottom: 8, display: 'flex', alignItems: 'center', gap: 10 }}>
                                    <span style={{ color: '#F87171' }}>✗</span> {c}
                                </li>
                            ))}
                        </ul>

                        <h3 style={{ color: '#FBBF24', fontSize: 24, margin: '0 0 10px 0' }}>🎯 最佳场景</h3>
                        <p style={{ fontSize: 26, fontWeight: 600, color: '#FDE68A', margin: 0 }}>
                            {framework.bestFor}
                        </p>
                    </div>
                </div>
            </div>
        </AbsoluteFill>
    );
};
