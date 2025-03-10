import './App.css';
import { useState } from 'react';

function App() {
  const [message, setMessage] = useState('');

  const handleCalibrate = () => {
    setMessage('Calibration started...');
    setTimeout(() => {
      setMessage('Calibration complete!');
    }, 2000);
  };

  return (
    <div>
        <button onClick={handleCalibrate} className="calibrate-button">
          Calibrate
        </button>
        {message && <p>{message}</p>}
    </div>
  );
}

export default App;
