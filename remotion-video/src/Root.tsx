import { Composition } from 'remotion';
import { Composition as MainComposition } from './Composition';
import './style.css';

export const RemotionRoot: React.FC = () => {
  return (
    <>
      <Composition
        id="MainVideo"
        component={MainComposition}
        durationInFrames={2460}
        fps={30}
        width={1920}
        height={1080}
      />
    </>
  );
};
