/*
Autór: Sergio Hernandez Castillo
Matrícula: A01025210
Descripción: Tarea 1 - Figuras con WebGL

Nota: Esta tarea fue basada del ejemplo 3d_cube
Nota: Las coordenadas de todos las figuras fueron mapeadas primero en GeoGebra 3D con el eje "y" puesto verticalmente
*/

let mat4 = glMatrix.mat4;
let projectionMatrix;
let shaderProgram, shaderVertexPositionAttribute, shaderVertexColorAttribute, shaderProjectionMatrixUniform, shaderModelViewMatrixUniform;
let duration = 10000; // ms

// Attributes: Input variables used in the vertex shader. Since the vertex shader is called on each vertex, these will be different every time the vertex shader is invoked.
// Uniforms: Input variables for both the vertex and fragment shaders. These do not change values from vertex to vertex.
// Varyings: Used for passing data from the vertex shader to the fragment shader. Represent information for which the shader can output different value for each vertex.
let vertexShaderSource =    
    "    attribute vec3 vertexPos;\n" +
    "    attribute vec4 vertexColor;\n" +

    "    uniform mat4 modelViewMatrix;\n" +
    "    uniform mat4 projectionMatrix;\n" +

    "    varying vec4 vColor;\n" +

    "    void main(void) {\n" +
    "		// Return the transformed and projected vertex value\n" +
    "        gl_Position = projectionMatrix * modelViewMatrix * \n" +
    "            vec4(vertexPos, 1.0);\n" +
    "        // Output the vertexColor in vColor\n" +
    "        vColor = vertexColor * 0.8;\n" +
    "    }\n";

// precision lowp float
// This determines how much precision the GPU uses when calculating floats. The use of highp depends on the system.
// - highp for vertex positions,
// - mediump for texture coordinates,
// - lowp for colors.
let fragmentShaderSource = 
    "    precision lowp float;\n" +
    "    varying vec4 vColor;\n" +
    "    void main(void) {\n" +
    "    gl_FragColor = vColor;\n" +
    "}\n";

function initWebGL(canvas){
    let gl = null;
    let msg = "Your browser does not support WebGL, " + "or it is not enabled by default.";
    
    try {
        gl = canvas.getContext("experimental-webgl");
    } 

    catch (e) {
        msg = "Error creating WebGL Context!: " + e.toString();
    }

    if (!gl) {
        alert(msg);
        throw new Error(msg);
    }

    return gl;        
}

function initViewport(gl, canvas){
    gl.viewport(0, 0, canvas.width, canvas.height);
}

function initGL(canvas){
    // Create a project matrix with 45 degree field of view
    projectionMatrix = mat4.create();
    
    mat4.perspective(projectionMatrix, Math.PI / 4, canvas.width / canvas.height, 1, 100);
    mat4.translate(projectionMatrix, projectionMatrix, [0, 0, -5]);
}

function createPyramid(gl, translation, rotationAxis){    
    // Vertex Data
    let vertexBuffer;
    vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    let verts = [
        // H, F, D, E, G (Base)
        0.0,  0.0, -1.5, // 0
        1.3, 0.0, -0.7, // 1
        0.7, 0.0, 0.7, // 2
        -0.7, 0.0, 0.7, // 3
        -1.3, 0.0, -0.7, // 4

        // D, E, I (Front)
        0.7, 0.0, 0.7, // 5
        -0.7, 0.0, 0.7, // 6
        0.0, 1.7, -0.3, // 7

        // G, E, I (Lower left)
        -1.3, 0.0, -0.7, // 8
        -0.7, 0.0, 0.7, // 9
        0.0, 1.7, -0.3, // 10

        // H, G, I, (Upper left)
        0.0,  0.0, -1.5, // 11
        -1.3, 0.0, -0.7, // 12
        0.0, 1.7, -0.3, // 13

        // H, F, I (Upper right)
        0.0,  0.0, -1.5, // 14
        1.3, 0.0, -0.7, // 15
        0.0, 1.7, -0.3, // 16

        // F, D ,I (Lower right)
        1.3, 0.0, -0.7, // 17
        0.7, 0.0, 0.7, // 18
        0.0, 1.7, -0.3, // 19
    ];

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);

    // Color data
    let colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);

    // This follows an RGBT code
    let faceColors = [
        [1.0, 0.0, 0.0, 1.0], // Upper right
        [0.0, 1.0, 0.0, 1.0], // Lower right
        [0.0, 0.0, 1.0, 1.0], // Front
        [1.0, 1.0, 0.0, 1.0], // Lower left
        [1.0, 0.0, 1.0, 1.0], // Upper left
        [0.0, 1.0, 1.0, 1.0]  // Base
    ];

    let vertexColors = [];
    let baseColors = [0.99, 0.80, 0.24, 1];

    for (let index = 0; index < 5; index++){
        vertexColors.push(...baseColors);
    }

    faceColors.forEach(color =>{
        for (let j=0; j < 3; j++){
            vertexColors.push(...color);
        }
    });

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexColors), gl.STATIC_DRAW);

    // Index data (defines the triangles to be drawn).
    let pyramidIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, pyramidIndexBuffer);

    let pyramidIndices = [
        0, 4, 3,    0, 3, 2,    0, 1, 2, // Base triangles - HGE, HED, HFD
        5, 6, 7, // Front face
        8, 9, 10, // Lower left
        11, 12, 13, // Upper left
        14, 15, 16, // Upper right
        17, 18, 19 // Lower right 
    ];

    // gl.ELEMENT_ARRAY_BUFFER: Buffer used for element indices.
    // Uint16Array: Array of 16-bit unsigned integers.
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(pyramidIndices), gl.STATIC_DRAW);
    
    let pyramid = {
        buffer: vertexBuffer, colorBuffer:colorBuffer, indices:pyramidIndexBuffer,
        vertSize:3, nVerts:20, colorSize:4, nColors: 20, nIndices:24,
        primtype:gl.TRIANGLES, modelViewMatrix: mat4.create(), currentTime : Date.now()
    };

    mat4.translate(pyramid.modelViewMatrix, pyramid.modelViewMatrix, translation);

    pyramid.update = function(){
        let now = Date.now();
        let deltat = now - this.currentTime;
        this.currentTime = now;
        let fract = deltat / duration;
        let angle = Math.PI * 2 * fract;
    
        // Rotates a mat4 by the given angle
        // mat4 out the receiving matrix
        // mat4 a the matrix to rotate
        // Number rad the angle to rotate the matrix by
        // vec3 axis the axis to rotate around
        mat4.rotate(this.modelViewMatrix, this.modelViewMatrix, angle, rotationAxis);
    };
    
    return pyramid;
}

function createDodecahedron(gl, translation, rotationAxis){
    // Vertex Data
    let vertexBuffer;
    vertexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    let verts = [
        // T, F, J, K, H (Base)
        -1.3, 0.0, -0.5, // 0
        -0.8, 0.8, -0.8, // 1
        0.0, 0.5, -1.3, // 2
        0.0, -0.5, -1.3, // 3
        -0.8, -0.8, -0.8, // 4

        // E, N, F, T, R (Lower middle, upper right)
        -0.8, 0.8, 0.8, // 5
        -0.5, 1.3, 0.0, // 6
        -0.8, 0.8, -0.8, // 7
        -1.3, 0.0, -0.5, // 8
        -1.3, 0.0, 0.5, // 9

        // M, B, J, F, N (Lower middle, lower right)
        0.5, 1.3, 0.0, // 10
        0.8, 0.8, -0.8, // 11
        0.0, 0.5, -1.3, // 12
        -0.8, 0.8, -0.8, // 13
        -0.5, 1.3, 0.0, // 14

        // S, D, K, J, B (Lower middle, front)
        1.3, 0.0, -0.5, // 15
        0.8, -0.8, -0.8, // 16
        0.0, -0.5, -1.3, // 17
        0.0, 0.5, -1.3, // 18
        0.8, 0.8, -0.8, // 19

        // O, P, H, K, D (Lower middle, lower left)
        0.5, -1.3, 0.0, // 20
        -0.5, -1.3, 0.0, // 21
        -0.8, -0.8, -0.8, // 22
        0.0, -0.5, -1.3, // 23
        0.8, -0.8, -0.8, // 24

        // G, R, T, H, P (Lower middle, upper left)
        -0.8, -0.8, 0.8, // 25
        -1.3, 0.0, 0.5, // 26
        -1.3, 0.0, -0.5, // 27
        -0.8, -0.8, -0.8, // 28
        -0.5, -1.3, 0.0, // 29 

        // A, Q, S, B, M (Upper middle, lower right)
        0.8, 0.8, 0.8, // 30
        1.3, 0.0, 0.5, // 31
        1.3, 0.0, -0.5, // 32
        0.8, 0.8, -0.8, // 33
        0.5, 1.3, 0.0, // 34

        // I, A, M, N, E (Upper middle, upper right)
        0.0, 0.5, 1.3, // 35
        0.8, 0.8, 0.8, // 36
        0.5, 1.3, 0.0, // 37
        -0.5, 1.3, 0.0, // 38
        -0.8, 0.8, 0.8, // 39

        // C, Q, S, D, O (Upper middle, lower left)
        0.8, -0.8, 0.8, // 40
        1.3, 0.0, 0.5, // 41
        1.3, 0.0, -0.5, // 42
        0.8, -0.8, -0.8, // 43
        0.5, -1.3, 0.0, // 44

        // L, C, O, P, G (Upper middle, upper left)
        0.0, -0.5, 1.3, // 45
        0.8, -0.8, 0.8, // 46
        0.5, -1.3, 0.0, // 47
        -0.5, -1.3, 0.0, // 48
        -0.8, -0.8, 0.8, // 49

        // I, L, G, R, E (Upper middle, back)
        0.0, 0.5, 1.3, // 50
        0.0, -0.5, 1.3, // 51
        -0.8, -0.8, 0.8, // 52
        -1.3, 0.0, 0.5, // 53
        -0.8, 0.8, 0.8, // 54

        // I, A, Q, C, L (Upper)
        0.0, 0.5, 1.3, // 55
        0.8, 0.8, 0.8, // 56
        1.3, 0.0, 0.5, // 57
        0.8, -0.8, 0.8, // 58
        0.0, -0.5, 1.3, // 59
    ];

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(verts), gl.STATIC_DRAW);

    // Color data
    let colorBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);

    let faceColors = [
        [1.0, 0.0, 0.0, 1.0],
        [0.0, 1.0, 0.0, 1.0],       
        [0.0, 0.0, 1.0, 1.0],       
        [1.0, 1.0, 0.0, 1.0],       
        [1.0, 0.0, 1.0, 1.0],       
        [0.0, 1.0, 1.0, 1.0],       
        [1.0, 1.0, 1.0, 1.0],         
        [0.0, 0.0, 0.0, 1.0],
        [1.0, 0.0, 0.0, 1.0],       
        [0.0, 1.0, 0.0, 1.0],       
        [0.0, 0.0, 1.0, 1.0],       
        [1.0, 1.0, 0.0, 1.0]      
    ];

    let vertexColors = [];

    faceColors.forEach(color => {
        for (let j = 0; j < 5; j++){
            vertexColors.push(...color);
        }
    });

    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexColors), gl.STATIC_DRAW);

    // Index data (defines the triangles to be drawn).
    let dodecahedronIndexBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, dodecahedronIndexBuffer);

    let dodecahedronIndices = [
       0, 1, 2,     0, 4, 3,    0, 3, 2, // Base triangles
       5, 6, 7,     5, 9, 8,    5, 8, 7, // Lower middle, upper right triangles
       10, 11, 12,  10, 14, 13,     10, 13, 12, // Lower middle, lower right triangles
       15, 16, 17,  15, 19, 18,     15, 18, 17, // Lower middle, front triangles
       20, 21, 22,  20, 24, 23,     20, 23, 22, // Lower middle, lower left triangles
       25, 26, 27,  25, 29, 28,     25, 28, 27, // Lower middle, upper left triangles
       30, 31, 32,  30, 34, 33,     30, 33, 32, // Upper middle, lower right triangles
       35, 36, 37,  35, 39, 38,     35, 38, 37, // Upper middle, upper right triangles 
       40, 41, 42,  40, 44, 43,     40, 43, 42, // Upper middle, lower left triangles
       45, 46, 47,  45, 49, 48,     45, 48, 47, // Upper middle, upper left triangles 
       50, 51, 52,  50, 54, 53,     50, 53, 52, // Upper middle, back triangles
       55, 56, 57,  55, 59, 58,     55, 58, 57, // Upper triangles
    ];

    // gl.ELEMENT_ARRAY_BUFFER: Buffer used for element indices.
    // Uint16Array: Array of 16-bit unsigned integers.
    gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(dodecahedronIndices), gl.STATIC_DRAW);

    // Change this 
    let dodecahedron = {
        buffer: vertexBuffer, colorBuffer: colorBuffer, indices: dodecahedronIndexBuffer,
        vertSize: 3, nVerts: 60, colorSize: 4, nColors: 60, nIndices: 108,
        primtype: gl.TRIANGLES, modelViewMatrix: mat4.create(), currentTime: Date.now()
    };

    mat4.translate(dodecahedron.modelViewMatrix, dodecahedron.modelViewMatrix, translation);

    dodecahedron.update = function () {
        let now = Date.now();
        let deltat = now - this.currentTime;
        this.currentTime = now;
        let fract = deltat / duration;
        let angle = Math.PI * 2 * fract;

        // Rotates a mat4 by the given angle
        // mat4 out the receiving matrix
        // mat4 a the matrix to rotate
        // Number rad the angle to rotate the matrix by
        // vec3 axis the axis to rotate around
        mat4.rotate(this.modelViewMatrix, this.modelViewMatrix, angle, rotationAxis);
    };

    return dodecahedron;
}

function createOctahedron(gl, translation, rotationAxis){

}

function createShader(gl, str, type){
    let shader;

    if (type == "fragment"){
        shader = gl.createShader(gl.FRAGMENT_SHADER);
    } 
    
    else if (type == "vertex"){
        shader = gl.createShader(gl.VERTEX_SHADER);
    } 
    
    else {
        return null;
    }

    gl.shaderSource(shader, str);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)){
        alert(gl.getShaderInfoLog(shader));
        return null;
    }

    return shader;
}

function initShader(gl){
    // load and compile the fragment and vertex shader
    let fragmentShader = createShader(gl, fragmentShaderSource, "fragment");
    let vertexShader = createShader(gl, vertexShaderSource, "vertex");

    // link them together into a new program
    shaderProgram = gl.createProgram();
    gl.attachShader(shaderProgram, vertexShader);
    gl.attachShader(shaderProgram, fragmentShader);
    gl.linkProgram(shaderProgram);

    // get pointers to the shader params
    shaderVertexPositionAttribute = gl.getAttribLocation(shaderProgram, "vertexPos");
    gl.enableVertexAttribArray(shaderVertexPositionAttribute);

    shaderVertexColorAttribute = gl.getAttribLocation(shaderProgram, "vertexColor");
    gl.enableVertexAttribArray(shaderVertexColorAttribute);
    
    shaderProjectionMatrixUniform = gl.getUniformLocation(shaderProgram, "projectionMatrix");
    shaderModelViewMatrixUniform = gl.getUniformLocation(shaderProgram, "modelViewMatrix");

    if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)){
        alert("Could not initialise shaders");
    }
}

function draw(gl, objs){
    // clear the background (with black)
    gl.clearColor(0.1, 0.1, 0.1, 1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    
    // set the shader to use
    gl.useProgram(shaderProgram);

    for(i = 0; i < objs.length; i++){
        obj = objs[i];
        // connect up the shader parameters: vertex position, color and projection/model matrices
        // set up the buffers
        gl.bindBuffer(gl.ARRAY_BUFFER, obj.buffer);
        gl.vertexAttribPointer(shaderVertexPositionAttribute, obj.vertSize, gl.FLOAT, false, 0, 0);

        gl.bindBuffer(gl.ARRAY_BUFFER, obj.colorBuffer);
        gl.vertexAttribPointer(shaderVertexColorAttribute, obj.colorSize, gl.FLOAT, false, 0, 0);
        
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, obj.indices);

        gl.uniformMatrix4fv(shaderProjectionMatrixUniform, false, projectionMatrix);
        gl.uniformMatrix4fv(shaderModelViewMatrixUniform, false, obj.modelViewMatrix);

        // Draw the object's primitives using indexed buffer information.
        // void gl.drawElements(mode, count, type, offset);
        // mode: A GLenum specifying the type primitive to render.
        // count: A GLsizei specifying the number of elements to be rendered.
        // type: A GLenum specifying the type of the values in the element array buffer.
        // offset: A GLintptr specifying an offset in the element array buffer.
        gl.drawElements(obj.primtype, obj.nIndices, gl.UNSIGNED_SHORT, 0);
    }
}

function run(gl, objs){
    // The window.requestAnimationFrame() method tells the browser that you wish to perform an animation and requests that the browser call a specified function to update an animation before the next repaint. The method takes a callback as an argument to be invoked before the repaint.
    requestAnimationFrame(function() { run(gl, objs); });

    draw(gl, objs);

    for(i = 0; i < objs.length; i++){
        objs[i].update();
    }
}