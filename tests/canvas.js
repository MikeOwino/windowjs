// Tests for the Canvas API: https://windowjs.org/doc/canvas
//
// Many of these tests compare the pixels in the canvas to a golden screenshot.
// The golden screenshots are generated with canvas.html.
// If this file is changed, then verify that canvas.html still works too.
//
// After adding a new test, open canvas.html to render its output in Chrome
// and save that canvas (right-click, Save image as...) as the golden for
// that test.

import {
  diffCanvasToFile,
} from './lib/lib.js';

export async function initialCanvasIsOpaqueBlack() {
  const canvas = window.canvas;
  canvas.fillStyle = 'black';
  canvas.fillRect(0, 0, canvas.width, canvas.height);
  await diffCanvasToFile('data/black.png');
}

export async function clearedCanvasIsTransparent() {
  const canvas = window.canvas;
  canvas.clearRect(0, 0, canvas.width, canvas.height);
  await diffCanvasToFile('data/transparent.png');
}

export async function opaqueWhiteFill() {
  const canvas = window.canvas;
  canvas.fillStyle = 'white';
  canvas.fillRect(0, 0, canvas.width, canvas.height);
  await diffCanvasToFile('data/white.png');
}

export async function fillRect() {
  const canvas = window.canvas;
  canvas.fillStyle = 'white';
  canvas.fillRect(0, 0, canvas.width, canvas.height);
  canvas.fillStyle = 'red';
  canvas.fillRect(0, 0, 50, 100);
  canvas.fillStyle = 'green';
  canvas.fillRect(50, 0, 50, 100);
  canvas.fillStyle = 'blue';
  canvas.fillRect(100, 0, 50, 100);
  canvas.fillStyle = '#4080c080';
  canvas.fillRect(150, 0, 50, 100);
  canvas.fillStyle = '#00000080';
  canvas.fillRect(0, 50, 200, 50);
  await diffCanvasToFile('data/fill_rect.png');
}

export async function arcs() {
  const canvas = window.canvas;
  canvas.fillStyle = 'red';
  canvas.arc(50, 50, 20, 0, Math.PI / 4 * 3);
  canvas.fill();
  canvas.fillStyle = 'green';
  canvas.beginPath();
  canvas.arc(100, 50, 20, 0, Math.PI / 4 * 3);
  canvas.fill();
  await diffCanvasToFile('data/arcs.png');
}