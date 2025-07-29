function make_environment(...envs) {
	return new Proxy(envs, {
		get(target, prop, receiver) {
			for(let env of envs) {
				if(env.hasOwnProperty(prop)) {
					return env[prop];
				}
			}
			return (...args) => { console.error("NOT IMPLEMENTED: "+prop, args); };
		}
	});
}

const libm = {
	"random": Math.random,
};

let iota = 0;
const CANVAS_CELLS       = iota++;
const CANVAS_FRAMEBUFFER = iota++;
const CANVAS_WIDTH       = iota++;
const CANVAS_HEIGHT      = iota++;
const CANVAS_SIZE        = iota++;

function readCanvasFromMemory(memory_buffer, canvas_ptr) {
	const canvas_memory = new Uint32Array(memory_buffer, canvas_ptr, CANVAS_SIZE);
	return {
		cells: canvas_memory[CANVAS_CELLS],
		framebuffer: canvas_memory[CANVAS_FRAMEBUFFER],
		width: canvas_memory[CANVAS_WIDTH],
		height: canvas_memory[CANVAS_HEIGHT],
	}
}

async function startGame() {
	const app = document.getElementById("app-game");
	if(app === null) {
	    console.error("failed to get element app-game");
	    return;
	}

	const ctx = app.getContext("2d");
	ctx.imageSmoothingEnabled = false;

	const w = await WebAssembly.instantiateStreaming(fetch("wasm/game.wasm"), {
		"env": make_environment(libm),
	});

	const heap_base = w.instance.exports.__heap_base.value;

	w.instance.exports.randomize(heap_base);

	let paused = false;

	const pauseButton = document.getElementById("game-button-pause");
	pauseButton.addEventListener("click", () => { paused = !paused });

	function step() {
		paused = true;
		render();
	}

	const stepButton = document.getElementById("game-button-step");
	stepButton.addEventListener("click", step);

	function randomize() {
		const buffer = w.instance.exports.memory.buffer;
		w.instance.exports.randomize(heap_base);
		const canvas = readCanvasFromMemory(buffer, heap_base);
		const image = new ImageData(new Uint8ClampedArray(buffer, canvas.cells, canvas.width * canvas.height * 4), canvas.width);

		app.width = canvas.width;
		app.height = canvas.height;

		ctx.putImageData(image, 0, 0);
	}

	const randomizeButton = document.getElementById("game-button-randomize");
	randomizeButton.addEventListener("click", randomize);

	const frameTimeSlider = document.getElementById("game-slider-frame-time");
	const frameTimeNumber = document.getElementById("frame-time-number");

	function render() {
		const buffer = w.instance.exports.memory.buffer;
		w.instance.exports.render(heap_base);
		const canvas = readCanvasFromMemory(buffer, heap_base);
		const image = new ImageData(new Uint8ClampedArray(buffer, canvas.cells, canvas.width * canvas.height * 4), canvas.width);

		// console.log(image);

		app.width = canvas.width;
		app.height = canvas.height;

		ctx.putImageData(image, 0, 0);
	}

	function loop() {
		if(!paused) render();
		frameTimeNumber.innerText = frameTimeSlider.value + " ms";
		setTimeout(loop, frameTimeSlider.value);
		// window.requestAnimationFrame(loop);
	}

	setTimeout(loop, frameTimeSlider.value);
	// window.requestAnimationFrame(loop);
}
