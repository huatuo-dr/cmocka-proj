import { Player, PlayerRef } from '@remotion/player';
import { useCallback, useEffect, useRef, useState } from 'react';
import { Composition } from './Composition';
import { SLIDE_TIMESTAMPS } from './data/frameworks';
import { ArrowLeft, ArrowRight, Pause, Play } from 'lucide-react';

export const InteractivePlayer: React.FC = () => {
    const playerRef = useRef<PlayerRef>(null);
    const [currentSlideIndex, setCurrentSlideIndex] = useState(0);
    const [isPlaying, setIsPlaying] = useState(false);

    // When set to a frame number, playback will auto-pause at that frame.
    // When null, playback runs freely (manual play mode).
    const autoStopFrame = useRef<number | null>(null);

    const durationInFrames = SLIDE_TIMESTAMPS[SLIDE_TIMESTAMPS.length - 1] + 300;

    // Called by Next/Prev: seek, auto-play, and set a stop boundary
    const seekToSlide = useCallback((index: number) => {
        if (index < 0 || index >= SLIDE_TIMESTAMPS.length) return;

        setCurrentSlideIndex(index);

        const targetFrame = SLIDE_TIMESTAMPS[index];
        playerRef.current?.seekTo(targetFrame);

        // Stop at the LAST frame of the current slide (not the first frame of next)
        const nextSlideStart = SLIDE_TIMESTAMPS[index + 1] ?? durationInFrames;
        autoStopFrame.current = nextSlideStart - 1;

        // Auto-play the current slide
        playerRef.current?.play();
        setIsPlaying(true);
    }, [durationInFrames]);

    const nextSlide = () => seekToSlide(currentSlideIndex + 1);
    const prevSlide = () => seekToSlide(currentSlideIndex - 1);

    // Manual play: clear autoStopFrame so it plays freely until the end
    const togglePlay = () => {
        if (isPlaying) {
            playerRef.current?.pause();
            setIsPlaying(false);
        } else {
            // Manual play = no auto-stop
            autoStopFrame.current = null;
            playerRef.current?.play();
            setIsPlaying(true);
        }
    };

    // Monitor frame updates to auto-pause at slide boundary
    useEffect(() => {
        const player = playerRef.current;
        if (!player) return;

        const handleFrameUpdate = (e: { detail: { frame: number } }) => {
            const currentFrame = e.detail.frame;
            const stopAt = autoStopFrame.current;

            if (stopAt !== null && currentFrame >= stopAt) {
                player.pause();
                autoStopFrame.current = null;

                const newIndex = SLIDE_TIMESTAMPS.findIndex(
                    (ts, i) => {
                        const next = SLIDE_TIMESTAMPS[i + 1] ?? Infinity;
                        return currentFrame >= ts && currentFrame < next;
                    }
                );
                if (newIndex >= 0) setCurrentSlideIndex(newIndex);
            }
        };

        // Sync button state with actual player state
        const handlePlay = () => setIsPlaying(true);
        const handlePause = () => setIsPlaying(false);

        player.addEventListener('frameupdate', handleFrameUpdate as any);
        player.addEventListener('play', handlePlay as any);
        player.addEventListener('pause', handlePause as any);
        return () => {
            player.removeEventListener('frameupdate', handleFrameUpdate as any);
            player.removeEventListener('play', handlePlay as any);
            player.removeEventListener('pause', handlePause as any);
        };
    }, []);

    // Keyboard controls
    useEffect(() => {
        const handleKeyDown = (e: KeyboardEvent) => {
            if (e.key === 'ArrowRight') nextSlide();
            if (e.key === 'ArrowLeft') prevSlide();
            if (e.key === ' ') {
                e.preventDefault();
                togglePlay();
            }
        };
        window.addEventListener('keydown', handleKeyDown);
        return () => window.removeEventListener('keydown', handleKeyDown);
    }, [currentSlideIndex, isPlaying]);

    return (
        <div style={{ fontFamily: 'Inter, sans-serif', backgroundColor: '#111', minHeight: '100vh', display: 'flex', flexDirection: 'column', alignItems: 'center', justifyContent: 'center' }}>
            <div style={{ boxShadow: '0 0 50px rgba(0,0,0,0.5)', borderRadius: 10, overflow: 'hidden' }}>
                <Player
                    ref={playerRef}
                    component={Composition}
                    durationInFrames={durationInFrames}
                    compositionWidth={1920}
                    compositionHeight={1080}
                    fps={30}
                    style={{ width: '80vw', aspectRatio: '16/9' }}
                    controls={false}
                />
            </div>

            {/* Control Bar */}
            <div style={{ marginTop: 20, display: 'flex', gap: 20, alignItems: 'center' }}>
                <button onClick={prevSlide} className="btn">
                    <ArrowLeft /> 上一页
                </button>

                <div style={{ color: 'white', fontSize: 20, fontWeight: 'bold' }}>
                    第 {currentSlideIndex + 1} 页 / 共 {SLIDE_TIMESTAMPS.length} 页
                </div>

                <button onClick={togglePlay} className="btn" style={{ padding: '15px 30px', backgroundColor: '#3182ce' }}>
                    {isPlaying ? <Pause /> : <Play />}
                </button>

                <button onClick={nextSlide} className="btn">
                    下一页 <ArrowRight />
                </button>
            </div>

            <style>{`
        .btn {
          background: #333;
          color: white;
          border: none;
          padding: 10px 20px;
          border-radius: 8px;
          cursor: pointer;
          display: flex;
          align-items: center;
          gap: 10px;
          font-size: 16px;
          transition: background 0.2s;
        }
        .btn:hover {
          background: #555;
        }
        .btn:active {
          transform: translateY(2px);
        }
      `}</style>
        </div>
    );
};
