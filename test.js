window.width = 600;
window.height = 600;

const canvas = window.canvas;

canvas.fillStyle = 'red';

// clearRect makes Screenshots transparent!
canvas.clearRect(0, 0, 600, 600);

canvas.fillStyle = '#ff0000ff';
canvas.fillStyle = '#ff000080';
canvas.fillRect(0, 0, 300, 300);
canvas.fillRect(300, 300, 300, 300);

// This is also transparent.
const png = await canvas.encode('png');
await File.write('encoded.png', png);
