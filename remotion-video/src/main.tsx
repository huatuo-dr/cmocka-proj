import React from 'react';
import ReactDOM from 'react-dom/client';
import { InteractivePlayer } from './InteractivePlayer';
import './style.css';

ReactDOM.createRoot(document.getElementById('root')!).render(
    <React.StrictMode>
        <InteractivePlayer />
    </React.StrictMode>
);
