import React from 'react';
import { AbsoluteFill, interpolate, useCurrentFrame } from 'remotion';
import { COMPARISON_FEATURES, FRAMEWORKS } from '../data/frameworks';

// Display all comparison features in a single table
export const ComparisonTable: React.FC = () => {
    const frame = useCurrentFrame();
    const opacity = interpolate(frame, [0, 20], [0, 1], { extrapolateRight: 'clamp' });

    const rows = COMPARISON_FEATURES;
    const frameworkIds = FRAMEWORKS.map((f) => f.id);
    const frameworkNames = FRAMEWORKS.map((f) => f.name);

    return (
        <AbsoluteFill
            style={{
                backgroundColor: '#0F172A',
                color: 'white',
                fontFamily: 'Inter, sans-serif',
                padding: '30px 40px',
                opacity,
            }}
        >
            {/* Title */}
            <h1 style={{ textAlign: 'center', fontSize: 40, margin: '0 0 20px 0', color: '#93C5FD' }}>
                📊 框架对比矩阵
            </h1>

            {/* Table */}
            <div
                style={{
                    display: 'grid',
                    gridTemplateColumns: `140px repeat(${frameworkIds.length}, 1fr)`,
                    gap: 0,
                    fontSize: 15,
                    width: '100%',
                }}
            >
                {/* Header row */}
                <div style={{ ...cellStyle, backgroundColor: '#1E3A5F', fontWeight: 'bold', color: '#93C5FD' }}>
                    特性
                </div>
                {frameworkNames.map((name) => (
                    <div key={name} style={{ ...cellStyle, backgroundColor: '#1E3A5F', fontWeight: 'bold', color: '#93C5FD', fontSize: 13 }}>
                        {name}
                    </div>
                ))}

                {/* Data rows */}
                {rows.map((row, rowIdx) => {
                    const rowDelay = rowIdx * 3;
                    const rowOpacity = interpolate(frame, [15 + rowDelay, 28 + rowDelay], [0, 1], {
                        extrapolateRight: 'clamp',
                    });
                    const bgColor = rowIdx % 2 === 0 ? '#1E293B' : '#0F172A';

                    return (
                        <React.Fragment key={row.feature}>
                            <div style={{ ...cellStyle, backgroundColor: bgColor, fontWeight: 600, opacity: rowOpacity }}>
                                {row.feature}
                            </div>
                            {frameworkIds.map((id) => {
                                const val = row.values[id] || '-';
                                const color = val.startsWith('✅') ? '#4ADE80' : val.startsWith('❌') ? '#F87171' : val.startsWith('⚠️') ? '#FBBF24' : '#E2E8F0';
                                return (
                                    <div key={id} style={{ ...cellStyle, backgroundColor: bgColor, color, opacity: rowOpacity, fontSize: 14 }}>
                                        {val}
                                    </div>
                                );
                            })}
                        </React.Fragment>
                    );
                })}
            </div>
        </AbsoluteFill>
    );
};

const cellStyle: React.CSSProperties = {
    padding: '7px 6px',
    borderBottom: '1px solid #334155',
    textAlign: 'center',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
};
