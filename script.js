var Module = Module || {};
Module = {
    onRuntimeInitialized: function() {
        console.log('WebAssembly module initialized');
        Module._init();
        requestAnimationFrame(updateFrame);
    }
};

let rotationX = 0, rotationY = 0, rotationZ = 0;
let Xaccel = 0, Yaccel = 0, Zaccel = 0;
let XVelocity = 0, YVelocity = 0, ZVelocity = 0;
let XTime = 0, YTime = 0, ZTime = 0;
let mode = 0, accel = 0.000001;
let distance = 150, light = true;
let halfw = window.innerWidth / 2, halfh = window.innerHeight / 2;
let rad = Math.sqrt(halfw * halfw + halfh * halfh);
let darkMode = false;
let cubeWidth = 20;

const fps = 24;
const frameInterval = 1000 / fps;
let lastFrameTime = 0;

function updateFrame(currentTime) {
    if (currentTime - lastFrameTime >= frameInterval){
        if (mode === 0) { // auto spin
            rotationX += 0.05;
            rotationY += 0.05;
            rotationZ += 0.01;
        } else if (mode === 2) { // acceleration control
            XTime++;
            YTime++;
            ZTime++;
            XVelocity += (Xaccel * XTime);
            YVelocity += (Yaccel * YTime);
            ZVelocity += (Zaccel * ZTime);
            rotationX += XVelocity;
            rotationY += YVelocity;
            rotationZ += ZVelocity;
        }
      
        Module._updateRotation(rotationX, rotationY, rotationZ);
        Module._updateDistance(distance);
        Module._updateCubeWidth(cubeWidth);
        Module._updateLight(light);
        Module._darkMode(darkMode);

        const height = Math.floor(window.innerHeight / (200 / cubeWidth));
        const width = Math.floor(window.innerWidth / (200 /cubeWidth));
        const bufferPtr = Module._renderFrame(height, width);
        const buffer = Module.HEAPU8.subarray(bufferPtr, bufferPtr + width * height);
        let frameContent = '';
        for (let y = 0; y < height; y++) {
            for (let x = 0; x < width; x++) {
                const char = String.fromCharCode(buffer[y * width + x]);
                frameContent += char;
            }
            frameContent += '\n';
        }
        
        document.getElementById('cubeCanvas').textContent = frameContent;
    
        //console.log('Buffer contents:', Array.from(buffer).map(b => String.fromCharCode(b)).join(''));
    
        lastFrameTime = currentTime;
    }
    requestAnimationFrame(updateFrame);
}


document.addEventListener('mousemove', (event) => {
    if (mode === 3) {
        halfw = window.innerWidth / 2, halfh = window.innerHeight / 2;
        rad = Math.sqrt(halfw * halfw + halfh * halfh);
        let mousex = event.clientX - halfw;
        let mousey = event.clientY - halfh;
        rotationY = -(Math.acos(mousex / rad)) + (Math.PI / 2);
        rotationX = Math.acos(mousey / rad);
    }
});

document.addEventListener('keydown', (event) => {
    if (mode === 1) {
        switch (event.key) {
            case 'w':
                rotationX += 0.1;
                break;
            case 's':
                rotationX -= 0.1;
                break;
            case 'a':
                rotationY -= 0.1;
                break;
            case 'd':
                rotationY += 0.1;
                break;
            case 'q':
                rotationZ += 0.1;
                break;
            case 'e':
                rotationZ -= 0.1;
                break;
        }
    } else if (mode === 2){
        switch (event.key) {
            case 'w':
                Yaccel += accel;
                YTime = 0;
                break;
            case 's':
                Yaccel -= accel;
                YTime = 0;
                break;
            case 'a':
                Xaccel -= accel;
                XTime = 0;
                break;
            case 'd':
                Xaccel += accel;
                XTime = 0;
                break;
            case 'q':
                Zaccel += accel;
                ZTime = 0;
                break;
            case 'e':
                Zaccel -= accel;
                ZTime = 0;
                break;
        }
    } 
    switch (event.key) {
        case 'r': // reset
            XVelocity = 0;
            Xaccel = 0;
            YVelocity = 0;
            Yaccel = 0;
            ZVelocity = 0;
            Zaccel = 0;
            XTime = 0;
            YTime = 0;
            ZTime = 0;
            rotationX = 0;
            rotationY = 0;
            rotationZ = 0;
            break;
        case 'm': // change mode
            mode = (mode + 1) % 4;
            document.getElementById('controlMode').value = mode;
            break;
        case 'l': // toggle light
            light = !light;
            document.getElementById('toggleLight').checked = light;
            break;
        case '=': // zoom in (plus button)
            if (distance-5 >= 90){
                distance -= 5;
                document.getElementById('distance').value = distance;
            }
            break;
        case '-': // zoom out (minus button)
            if (distance+5 <= 300){
                distance += 5;
                document.getElementById('distance').value = distance;
                
            }
            break;
    }
});

document.getElementById('toggleLight').addEventListener('change', (event) => {
    light = event.target.checked;
});

document.getElementById('distance').addEventListener('input', (event) => {
    distance = event.target.value;
});

document.getElementById('resolution').addEventListener('input', (event) => {
    cubeWidth = event.target.value;
    document.getElementById('cubeCanvas').style.fontSize = `${320 / cubeWidth}px`;
});

const darkModeToggle = document.getElementById('darkMode');
        darkModeToggle.addEventListener('change', (event) => {
            darkMode = event.target.checked;
            if (event.target.checked) {
                document.body.style.backgroundColor = '#333';
                document.body.style.color = '#fff';
            } else {
                document.body.style.backgroundColor = '#fcf6e5';
                document.body.style.color = '#000';
            }
        });

const switchMode = document.getElementById('controlMode');
switchMode.addEventListener('change', (event) => {
    mode = parseInt(event.target.value, 10);
    console.log('Mode:', mode);
});

const resetButton = document.getElementById('reset');
resetButton.addEventListener('click', () => {
    XVelocity = 0;
    Xaccel = 0;
    YVelocity = 0;
    Yaccel = 0;
    ZVelocity = 0;
    Zaccel = 0;
    XTime = 0;
    YTime = 0;
    ZTime = 0;
    rotationX = 0;
    rotationY = 0;
    rotationZ = 0;
});


document.getElementById('light-checkmark').addEventListener('click', function() {  
    light = !light;
    document.getElementById('toggleLight').checked = light;
});

document.getElementById('darkmode-checkmark').addEventListener('click', function() {  
    darkMode = !darkMode;
    document.getElementById('darkMode').checked = darkMode;
    if (darkMode) {
        document.body.style.backgroundColor = '#333';
        document.body.style.color = '#fff';
    } else {
        document.body.style.backgroundColor = '#fcf6e5';
        document.body.style.color = '#000';
    }
});