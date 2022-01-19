await window.loadFont('examples/data/OleoScriptSwashCaps.ttf',
                      'Oleo Script Swash Caps');

window.canvas.fillStyle = '#eb005a';
window.canvas.font = '64px "Oleo Script Swash Caps"';
window.canvas.fillText('Window.js', 16, 128);
window.canvas.fillRect(0, 300, 100, 100);
