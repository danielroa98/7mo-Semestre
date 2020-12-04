/*
 *  Happy Feet 3D - Electric Bungaloo
 * 
 *  Gráficas computacionales
 *      Octavio Navarro
 * 
 *  Sebastián Vives    -    A01025211
 *  Simón Metta        -    A01377925
 *  Daniel Roa         -    A01021960
 */
let renderer = null;
let scene = null;
let camera = null;

let score = 0;

let materials = null;

let floorUniforms = null;

let objLoader = null, mtlLoader = null;

let objModelUrl = { obj: '../models/pingu.obj', map: '../models/pingu_tex.bmp' };
let shipHolder = null;

let enemy = null;
pigUrl = { obj: '../models/pigeon.obj', map: '../models/pigeon_tex.jpg' };

let wKey = false, sKey = false, aKey = false, dKey = false;

let moveSpeed = 33;

let ObjectBolillo;

let vida = null;
let vidaText = null;

let projectilesCounter = [];
let bolsaBolillos = [];
let enemigos = [];

let currentTime = Date.now();

let batallions = [];

let timer = 40;
let waveTimer = 10;
let dinoTimer = 100;
let marioTimer = 20;

let gallinas = [];
let kevins = [];
let balasKevin = [];
let fishes = [];
let dinos = [];

//Load fish
let objFish = "./models/Fish/fish.obj";
let mtlFish = "../models/Fish/fish.mtl";

//Load chicken with gun .obj and .mtl
let objChickenGun = "./models/Chickens/Pistola/chicken_w_gun.obj";
let mtlChickenGun = "../models/Chickens/Pistola/chicken_w_gun.mtl";

//Load chicken with knife .obj and .mtl
let objChickenKnife = "./models/Chickens/Cuchillo/chicken_knife.obj";
let mtlChickenKnife = "./models/Chickens/Cuchillo/chicken_knife.mtl";

//Load chicken with knife .obj and .mtl
let objBolillo = "./models/Bolillo/Loaf of Bread for Export.obj";
let mtlBolillo = "./models/Bolillo/Loaf of Bread for Export.mtl";

let objKnife = "./models/Knife/Kitchen_knife.obj";
let mtlKnife = "./models/Knife/Kitchen_knife.mtl";

//Load Dinosaur
let dinoObj = "./models/Dino/Dino.obj";
let dinoMtl = "./models/Dino/Dino.mtl";

//Load Mario
let marioObj = "./models/Mario-File/Mario.obj";
let marioMtl = "./models/Mario-File/Mario.mtl";

//Load SantaHat
let santaObj = "./models/Santa-hat/clowp.OBJ";
let santaMtl = "./models/Santa-hat/clowp.mtl";

//Enemy models
enemyModels = [
    { modelo: objChickenGun, textura: mtlChickenGun },
    { modelo: objChickenKnife, textura: mtlChickenKnife },
    { modelo: dinoObj, textura: dinoMtl }
];

// Function used to change the key staus when one of these is pressed down and released later on
function initControls() {
    document.addEventListener("keydown", (e) => {
        if (e.key == "d") {
            dKey = true;
        } else if (e.key == "a") {
            aKey = true;
        } else if (e.key == "w") {
            wKey = true;
        } else if (e.key == "s") {
            sKey = true;
        }
    });

    document.addEventListener("keyup", (e) => {
        if (e.key == "d") {
            dKey = false;
        } else if (e.key == "a") {
            aKey = false;
        } else if (e.key == "w") {
            wKey = false;
        } else if (e.key == "s") {
            sKey = false;
        }
    });

    document.addEventListener("keypress", (e) => {
        if (e.key == " ") {
            shoot();
        }
    });
}

function promisifyLoader(loader, onProgress) {
    function promiseLoader(url) {
        return new Promise((resolve, reject) => {
            loader.load(url, resolve, onProgress, reject);
        });
    }

    return {
        originalLoader: loader,
        load: promiseLoader,
    };
}

// Function used to load the background and its image
function loadMaterials() {
    let spaceTexture = new THREE.TextureLoader().load("./img/spaaaace.jpeg");

    floorUniforms = {
        time: { type: "f", value: 0.2 },
        glowTexture: { type: "t", value: spaceTexture }
    };
    floorUniforms.glowTexture.value.wrapS = floorUniforms.glowTexture.value.wrapT = THREE.RepeatWrapping;

    materials = {
        walls: new THREE.ShaderMaterial({
            uniforms: floorUniforms,
            vertexShader: document.getElementById('vertexShader').textContent,
            fragmentShader: document.getElementById('fragmentShader').textContent,
            side: THREE.DoubleSide
        }),
        floor: new THREE.ShaderMaterial({
            uniforms: floorUniforms,
            vertexShader: document.getElementById('vertexShader').textContent,
            fragmentShader: document.getElementById('fragmentShader').textContent,
            side: THREE.DoubleSide
        })
    }
}

// Function used to load objects that don't require an .mtl file to load textures
async function loadObj(objModelUrl, holder, scale, zPos, yPos, xPos, xRot, yRot) {
    const objPromiseLoader = promisifyLoader(new THREE.OBJLoader());

    try {
        const object = await objPromiseLoader.load(objModelUrl.obj);

        let texture = objModelUrl.hasOwnProperty('map') ? new THREE.TextureLoader().load(objModelUrl.map) : null;

        object.traverse(function (child) {
            if (child instanceof THREE.Mesh) {
                child.material.map = texture;
                child.scale.set(scale, scale, scale);
                child.rotation.x = xRot;
                child.rotation.y = yRot;
            }
        });

        holder.position.z = zPos;
        holder.position.y = yPos;
        holder.position.x = xPos;
        holder.add(object);

    }
    catch (err) {
        return onError(err);
    }
}

// Function used to load objects with their respective materials.
function loadObjWithMtl(enemyModels, positions, rotations, size, array, isBullet, vida) {
    mtlLoader = new THREE.MTLLoader();

    let enemyModelUrl = enemyModels.modelo;
    let mtlModelEnemy = enemyModels.textura;

    // console.log(loadedEnemy);
    //console.log(enemyModelUrl);
    //console.log(mtlModelEnemy);

    mtlLoader.load(mtlModelEnemy, materials => {

        materials.preload();
        //console.log(materials);

        objLoader = new THREE.OBJLoader();

        objLoader.setMaterials(materials);

        objLoader.load(enemyModelUrl, object => {
            object.traverse(function (child) {
                if (child.isMesh) {
                    //child.geometry.computeVertexNormals();
                    child.geometry.computeBoundingBox();
                }
            });

            //objectList.push(object);
            object.rotation.x = rotations[0];
            object.rotation.y = rotations[1];
            object.rotation.z = rotations[2];
            object.position.x = positions[0];
            object.position.y = positions[1];
            object.scale.set(size, size, size);
            object.posBool = false;
            object.timer = 5;
            object.vida = vida;
            if (isBullet) {
                array.push({ obj: object, life: Date.now() });
            }
            else {
                array.push(object);
            }

            if (enemyModels.side != null) {
                object.side = enemyModels.side;
                //console.log(object.side);
            }

            scene.add(object);
        });
    });
}

// Function used to load the GameOver text as soon as the player's health equals 0
function endTitleText(){
    var restartGame = document.createElement('div');
    restartGame.style.position = 'absolute';
    restartGame.style.width = 16 + 'em';
    restartGame.innerHTML = 'Reload this screen to try again';
    restartGame.style.bottom = 200 + 'px';
    restartGame.style.left = 15 + 'px';
    restartGame.style.fontSize = 50 + 'px';
    restartGame.style.color = '#ffffff';
    restartGame.id = "restartGame";
    document.body.appendChild(restartGame);

    var gameOver = document.createElement('div');
    gameOver.style.position = 'absolute';
    gameOver.style.width = 8 + 'em';
    gameOver.innerHTML = 'GAMEOVER';
    gameOver.style.bottom = 250 + 'px';
    gameOver.style.left = 15 + 'px';
    gameOver.style.fontSize = 60 + 'px';
    gameOver.style.color = '#ffffff';
    gameOver.id = "finit";
    document.body.insertBefore(gameOver, restartGame);
}

// Function used to start running the scene
// As soon as the player's health is equal to 0, then the scene will stop and it will render the Game Over text
function run() {

    if (vidaText != 0) {
        requestAnimationFrame(() => { run(); });

    } else {
        endTitleText();
    }

    // Render the scene
    renderer.render(scene, camera);

    KF.update();

    animate();

    //console.log()

}

// Function used to create enemy groups,
// @num - amount of enemies that will be rendered in a batallion
function createBatallion(num) {
    for (let i = 0; i < num; i++) {
        let temp = new THREE.Object3D();

        let enemyPos = (Math.random() * (-5)) + 5;

        loadObj(pigUrl, temp, 0.25, 0.55, 8, i, Math.PI / 2, 0);

        //console.log(enemyPos);

        scene.add(temp);
        temp.posBool = false;
        temp.posBoolY = false;
        temp.timer = 10 + (Math.random() * 1) + 5;
        batallions.push(temp);

    }
}

// Function in charge of adding and creating a scene, in this function, we are adding enemies, the space ship and the text that's seen on the canvas
// @canvas - value received from the game.html file, it lets this script know where it has to render it's elements.
function createScene(canvas) {
    // Create the Three.js renderer and attach it to our canvas
    renderer = new THREE.WebGLRenderer({ canvas: canvas, antialias: true });

    // Set the viewport size
    renderer.setSize(canvas.width, canvas.height);

    // Turn on shadows
    renderer.shadowMap.enabled = true;
    // Options are THREE.BasicShadowMap, THREE.PCFShadowMap, PCFSoftShadowMap
    renderer.shadowMap.type = THREE.BasicShadowMap;

    loadMaterials();

    // Create a new Three.js scene
    scene = new THREE.Scene();
    scene.background = new THREE.Color('black');

    // Add  a camera so we can view the scene
    camera = new THREE.PerspectiveCamera(45, canvas.width / canvas.height, 1, 4000);
    camera.position.set(0, -15, 15);
    camera.rotation.x = 365.2;
    scene.add(camera);

    let directionalLight = new THREE.DirectionalLight(0xffffff, 1);
    directionalLight.position.set(0, 50, 50);
    directionalLight.target.position.set(0, 0, 0);
    scene.add(directionalLight);

    let ambientLight = new THREE.AmbientLight(0xffffff, 0.6);
    scene.add(ambientLight);

    shipHolder = new THREE.Object3D();
    loadObj(objModelUrl, shipHolder, 0.1, 1, -8, 0, 0, Math.PI / 2);

    enemy = new THREE.Object3D();

    // console.log(batallions);

    // console.log(enemyModels);

    scene.add(shipHolder);

    let spaceGeometry = new THREE.CylinderGeometry(20, 20, 100, 30, 1, true, 0, 4);
    let space = new THREE.Mesh(spaceGeometry, materials.walls);
    space.rotation.y = Math.PI / 3
    space.position.y = 20;
    scene.add(space);

    let loadedEnemy = Math.floor((Math.random() * 2) + 0);

    let enemyModelUrl = enemyModels[loadedEnemy].modelo;
    let mtlModelEnemy = enemyModels[loadedEnemy].textura;

    //Icon
    var heart = document.createElement('div');
    heart.style.position = 'absolute';
    heart.style.width = 100;
    heart.style.height = 100;
    heart.innerHTML = "♡";
    heart.style.top = 7.5 + 'px';
    heart.style.left = 50 + 'px';
    heart.style.fontSize = 100 + 'px';
    heart.style.color = '#fc3003';
    document.body.insertBefore(heart, text2);

    //Vida
    var text2 = document.createElement('div');
    text2.style.position = 'absolute';
    //text2.style.zIndex = 1;    // if you still don't see the label, try uncommenting this
    text2.style.width = 100;
    text2.style.height = 100;
    text2.innerHTML = 100;
    text2.style.top = 20 + 'px';
    text2.style.left = 110 + 'px';
    text2.style.fontSize = 100 + 'px';
    text2.style.color = '#fc3003';
    text2.id = "vida";
    document.body.appendChild(text2);

    //Score text
    var pointScore = document.createElement('div');
    pointScore.style.position = 'absolute';
    pointScore.style.width = 100;
    pointScore.style.height = 100;
    pointScore.innerHTML = 'Score: ';
    pointScore.style.top = 20 + 'px';
    pointScore.style.right = 200 + 'px';
    pointScore.style.fontSize = 60 + 'px';
    pointScore.style.color = '#e8e8e8';
    pointScore.id = "pointScore";
    document.body.insertBefore(pointScore, points);

    //Scores
    var points = document.createElement('div');
    points.style.position = 'absolute';
    points.style.width = 100;
    points.style.height = 100;
    points.innerHTML = score;
    points.style.top = 20 + 'px';
    points.style.right = 5 + 'px';
    points.style.fontSize = 60 + 'px';
    points.style.color = '#e8e8e8';
    points.id = "points";
    document.body.appendChild(points);

}

// Function in charge of rendering and moving the player (based on the keyboard's input), the enemies, their positions in the scene (in the case of the enemies), and the shots fired from the elements found in-game.
function animate() {
    let now = Date.now();
    let deltat = now - currentTime;
    currentTime = now;
    let fract = deltat / 50;

    timer -= 0.1;
    waveTimer -= 0.1;
    dinoTimer -= 0.1;

    if (dinoTimer <= 0 && dinos.length == 0) {
        dinoTimer = 200;
        spawnDino();
    }

    //console.log(waveTimer);
    if (timer <= 0) {
        timer = 40 - (score * 0.01);
        createBatallion(10);
    }

    //Create random Dino or Chicken
    if (waveTimer <= 0) {
        waveTimer = 40 - (score * 0.01);
        loadObjWithMtl(enemyModels[1], [0, 10, 0], [Math.PI / 2, 0, 0], 1.5, gallinas, 0, 1);
    }


    dinos.forEach((element, index, array) => {
        marioTimer -= 0.1;
        if (marioTimer <= 0) {
            shootMario(element);
            marioTimer = 50;
        }
    })


    gallinas.forEach((element, index) => {
        let upPosition = 0.06;
        if (element.posBool == false) {
            element.position.x -= upPosition;
        }
        else if (element.posBool == true) {
            element.position.x += upPosition;
        }

        if (element.position.x <= -13) {
            element.posBool = true;
            shootKevin(element, 1.5);

        }
        else if (element.position.x >= 13) {
            element.posBool = false;
            shootKevin(element, 4.6);

        }


    });

    kevins.forEach((element, index, array) => {
        if (Date.now() - element.life > 4800) {
            scene.remove(element.obj);
            array.splice(index, 1);
        }
        //console.log(element.obj.timer);

        if (element.obj.timer <= 0) {
            element.obj.timer = 15;

            if (element.obj.position.x < 0) {
                loadObjWithMtl({ modelo: objKnife, textura: mtlKnife, side: 0 }, [element.obj.position.x, element.obj.position.y, element.obj.position.z], [element.obj.rotation.x, element.obj.rotation.y - 1.5, element.obj.rotation.z], 4, balasKevin, 1);
            }
            else {
                loadObjWithMtl({ modelo: objKnife, textura: mtlKnife, side: 1 }, [element.obj.position.x, element.obj.position.y, element.obj.position.z], [element.obj.rotation.x, element.obj.rotation.y - 1.5, element.obj.rotation.z], 4, balasKevin, 1);

            }

        }
        else {
            element.obj.timer -= 0.2;
        }

        element.obj.position.y -= fract * 0.2;

    });

    balasKevin.forEach((element, index, array) => {
        if (Date.now() - element.life > 4000) {
            scene.remove(element.obj);
            array.splice(index, 1);
        }

        //Collision detecion del bolillo (bolillo -> pingu)
        if (element.obj.position.x <= (shipHolder.position.x + 1.5)
            && element.obj.position.x >= (shipHolder.position.x - 1.5)
            && element.obj.position.y <= (shipHolder.position.y + 1)
            && element.obj.position.y >= (shipHolder.position.y - 1)
            && element.obj.position.z <= (shipHolder.position.z + 1)
            && element.obj.position.z >= (shipHolder.position.z - 1)) {

            //Se detecta la colision
            scene.remove(element.obj);
            array.splice(index, 1);
            vida = document.getElementById("vida");
            vidaText = vida.innerHTML - 10;

            vida.innerHTML = vidaText;
        }

        //Como se llama cada frame, actualizo su posicion
        if (element.obj.side == 0) {
            element.obj.position.x += fract * 0.3;
        }
        else if (element.obj.side == 1) {
            element.obj.position.x -= fract * 0.3;
        }


    })

    let scoreText = document.getElementById("points");

    if (dKey && wKey) {
        if (shipHolder.position.x < 12 && shipHolder.position.y < 7) {
            moveRightUp();
        }
    } else if (dKey && sKey) {
        if (shipHolder.position.x < 12 && shipHolder.position.y > -9) {
            moveRightDown();
        }
    } else if (aKey && wKey) {
        if (shipHolder.position.x > -12 && shipHolder.position.y < 7) {
            moveLeftUp();
        }
    } else if (aKey && sKey) {
        if (shipHolder.position.x > -12 && shipHolder.position.y > -9) {
            moveLeftDown();
        }
    } else if (dKey) {
        if (shipHolder.position.x < 12) {
            moveRight();
        }
    } else if (aKey) {
        if (shipHolder.position.x > -12) {
            moveLeft();
        }
    } else if (wKey) {
        if (shipHolder.position.y < 7) {
            moveUp();
        }
    } else if (sKey) {
        if (shipHolder.position.y > -9) {
            moveDown();
        }
    }

    //Controlador de pescados
    fishes.forEach((fish, index, array) => {
        if (Date.now() - fish.life > 4000) {
            scene.remove(fish.obj);
            array.splice(index, 1);
        }

        //Collision detecion del bolillo (bolillo -> pingu)
        if (fish.obj.position.x <= (shipHolder.position.x + 1.5)
            && fish.obj.position.x >= (shipHolder.position.x - 1.5)
            && fish.obj.position.y <= (shipHolder.position.y + 1)
            && fish.obj.position.y >= (shipHolder.position.y - 1)
            && fish.obj.position.z <= (shipHolder.position.z + 1)
            && fish.obj.position.z >= (shipHolder.position.z - 1)) {

            //Se detecta la colision
            scene.remove(fish.obj);
            array.splice(index, 1);
            vida = document.getElementById("vida");
            if (parseInt(vida.innerHTML) <= 90) {
                vidaText = parseInt(vida.innerHTML) + 10;
                vida.innerHTML = vidaText;
            }

        }

        fish.obj.position.y -= 0.1;
        fish.obj.rotation.z += 0.1;
    })

    //Controlador de los bolillos
    bolsaBolillos.forEach((bolillo, index, array) => {
        //Vida de l Bolillo
        if (Date.now() - bolillo.life > 2000) {
            scene.remove(bolillo.obj);
            array.splice(index, 1);
        }

        //Collision detecion del bolillo (bolillo -> pingu)
        if (bolillo.obj.position.x <= (shipHolder.position.x + 1.5)
            && bolillo.obj.position.x >= (shipHolder.position.x - 1.5)
            && bolillo.obj.position.y <= (shipHolder.position.y + 1)
            && bolillo.obj.position.y >= (shipHolder.position.y - 1)
            && bolillo.obj.position.z <= (shipHolder.position.z + 1)
            && bolillo.obj.position.z >= (shipHolder.position.z - 1)) {

            //Se detecta la colision
            scene.remove(bolillo.obj);
            array.splice(index, 1);
            vida = document.getElementById("vida");
            vidaText = vida.innerHTML - 10;

            vida.innerHTML = vidaText;
        }

        //Como se llama cada frame, actualizo su posicion
        bolillo.obj.position.y -= fract * 0.5
        bolillo.obj.rotation.y += 0.3;
    })

    projectilesCounter.forEach((proj, index, array) => {
        if (Date.now() - proj.life > 1000) {
            scene.remove(proj.obj);
            array.splice(index, 1);
        }
        //Calculo de colision con las palomas
        batallions.forEach((paloma, index2, array2) => {
            //Ver boundries
            if (proj.obj.position.x <= (paloma.position.x + 0.5)
                && proj.obj.position.x >= (paloma.position.x - 0.5)
                && proj.obj.position.y <= (paloma.position.y + 0.5)
                && proj.obj.position.y >= (paloma.position.y - 0.5)
                && proj.obj.position.z <= (paloma.position.z + 10)
                && proj.obj.position.z >= (paloma.position.z - 10)) {
                //console.log("Le di a paloma " + index2);
                //loadObjWithMtl({model: objFish, texture: mtlFish}, [paloma.position.x,paloma.position.y,paloma.position.z], [0,0,0], 1.5, fishes, 0,0);

                if (Math.random() < 0.1) {
                    spawnfish(paloma);
                }

                scene.remove(paloma);
                scene.remove(proj.obj);
                array.splice(index, 1);
                array2.splice(index2, 1);
                score += 1;
                scoreText.innerHTML = score;
            }
        });
        //Calculo de colision con las gallinas
        gallinas.forEach((paloma, index2, array2) => {
            //Ver boundries
            if (proj.obj.position.x <= (paloma.position.x + 0.5)
                && proj.obj.position.x >= (paloma.position.x - 0.5)
                && proj.obj.position.y <= (paloma.position.y + 0.5)
                && proj.obj.position.y >= (paloma.position.y - 0.5)
                && proj.obj.position.z <= (paloma.position.z + 10)
                && proj.obj.position.z >= (paloma.position.z - 10)) {
                //console.log("Le di a paloma " + index2);
                if (paloma.vida > 0) {
                    paloma.vida -= 1;
                }
                else {
                    scene.remove(paloma);
                    scene.remove(proj.obj);
                    array.splice(index, 1);
                    array2.splice(index2, 1);
                    score += 10;
                    scoreText.innerHTML = score;


                }

            }
        });

        //Calculo de colision con las gallinas
        bolsaBolillos.forEach((paloma, index2, array2) => {
            //Ver boundries
            if (proj.obj.position.x <= (paloma.obj.position.x + 0.5)
                && proj.obj.position.x >= (paloma.obj.position.x - 0.5)
                && proj.obj.position.y <= (paloma.obj.position.y + 0.5)
                && proj.obj.position.y >= (paloma.obj.position.y - 0.5)
                && proj.obj.position.z <= (paloma.obj.position.z + 10)
                && proj.obj.position.z >= (paloma.obj.position.z - 10)) {
                //console.log("Le di a paloma " + index2);
                if (paloma.vida > 0) {
                    paloma.vida -= 1;
                }
                else {
                    scene.remove(paloma.obj);
                    scene.remove(proj.obj);
                    array.splice(index, 1);
                    array2.splice(index2, 1);
                    score += 10;
                    scoreText.innerHTML = score;
                }

            }
        });

        //Calculo de colision con las gallinas
        dinos.forEach((paloma, index2, array2) => {
            //Ver boundries
            if (proj.obj.position.x <= (paloma.position.x + 5)
                && proj.obj.position.x >= (paloma.position.x - 5)
                && proj.obj.position.y <= (paloma.position.y + 5)
                && proj.obj.position.y >= (paloma.position.y - 5)
                && proj.obj.position.z <= (paloma.position.z + 5)
                && proj.obj.position.z >= (paloma.position.z - 5)) {
                //console.log("VIDA DINO" + paloma.vida);
                if (paloma.vida > 0) {
                    paloma.vida -= 1;
                    scene.remove(proj.obj);
                    array.splice(index, 1);
                }
                else {
                    scene.remove(paloma);
                    scene.remove(proj.obj);
                    array.splice(index, 1);
                    array2.splice(index2, 1);
                    score += 20;
                    scoreText.innerHTML = score;
                }

            }
        });

        proj.obj.position.y += fract * 3
    });

    //Probabilidad de que paloma dispare un pan
    batallions.forEach((paloma, index, array) => {
        let temp = Math.random();
        if (paloma.timer <= 0) {
            if (temp <= 0.25) {
                //console.log("Paloma " + index + " lanza un pan");
                shootBolillo(paloma);
            }
            paloma.timer = 30 + (Math.random() * 1) + 10;
        }
        else {
            paloma.timer -= 0.1;
        }

        //Collision detecion del bolillo (bolillo -> pingu)
        if( (shipHolder.position.x + 1.5) >= paloma.position.x
            &&  (shipHolder.position.x - 1.5) <= paloma.position.x
            &&  (shipHolder.position.y + 1) >= paloma.position.y
            &&  (shipHolder.position.y - 1) <= paloma.position.y
            &&  (shipHolder.position.z + 1) >= paloma.position.z
            &&  (shipHolder.position.z - 1) <= paloma.position.z){
                
                //Se detecta la colision
                scene.remove(paloma);
                array.splice(index, 1);
                vida = document.getElementById("vida");
                vidaText = vida.innerHTML - 10;

                vida.innerHTML = vidaText; 
            }

    });

    for (let x = 0; x < batallions.length; x++) {
        const element = batallions[x];

        let upPosition = 0.05 + (0.00001 * score);
        if (element.posBool == false) {
            element.position.x -= upPosition;
        }
        else if (element.posBool == true) {
            element.position.x += upPosition;
        }

        if (element.position.x <= -13) {
            element.posBool = true;
            element.position.y -= 1;
        }
        else if (element.position.x >= 13) {
            element.posBool = false;
            element.position.y -= 1;
        }

    }

    floorUniforms.time.value += fract / 10;
}

//Functions handling the ship's movement are found from this point until line 960
function moveRight() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { x: shipHolder.position.x },
                        { x: shipHolder.position.x + 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveLeft() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { x: shipHolder.position.x },
                        { x: shipHolder.position.x - 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveUp() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { y: shipHolder.position.y },
                        { y: shipHolder.position.y + 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveDown() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { y: shipHolder.position.y },
                        { y: shipHolder.position.y - 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveRightUp() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { x: shipHolder.position.x, y: shipHolder.position.y },
                        { x: shipHolder.position.x + 1, y: shipHolder.position.y + 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveRightDown() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { x: shipHolder.position.x, y: shipHolder.position.y },
                        { x: shipHolder.position.x + 1, y: shipHolder.position.y - 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveLeftUp() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { x: shipHolder.position.x, y: shipHolder.position.y },
                        { x: shipHolder.position.x - 1, y: shipHolder.position.y + 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}

function moveLeftDown() {
    let move = new KF.KeyFrameAnimator;
    move.init({
        interps:
            [
                {
                    keys: [0, 1],
                    values: [
                        { x: shipHolder.position.x, y: shipHolder.position.y },
                        { x: shipHolder.position.x - 1, y: shipHolder.position.y - 1 },
                    ],
                    target: shipHolder.position
                },
            ],
        loop: false,
        duration: moveSpeed
    });
    move.start();
}
//Until here the functions handling the ship's movement

//Function in charge of shooting the ship's bullets
function shoot() {
    let geometry = new THREE.SphereGeometry(0.2, 32, 32);
    let material = new THREE.MeshBasicMaterial({ color: "red" });

    let projectile = new THREE.Mesh(geometry, material);

    projectile.position.set(shipHolder.position.x, shipHolder.position.y, shipHolder.position.z);
    projectilesCounter.push({ obj: projectile, life: Date.now() });
    scene.add(projectile);
    //console.log(projectilesCounter);

}

//Function in charge of shooting bread from the pidgeon enemy-type.
//@paloma - receives information from the pidgeon in order to place correctly the projectile.
function shootBolillo(paloma) {

    let bolillo = {
        modelo: objBolillo,
        textura: mtlBolillo
    }

    loadObjWithMtl(bolillo, [paloma.position.x, paloma.position.y, paloma.position.z], [paloma.rotation.x, paloma.rotation.y, 90], 5, bolsaBolillos, 1);
    //console.log(bolsaBolillos);

}

//Function in charge of loading a fish in order to heal the player.
//@paloma - receives the position of a pidgeon in order to load a fish in it's place, once its shot by the player.
function spawnfish(paloma) {

    let fish = {
        modelo: objFish,
        textura: mtlFish
    }

    loadObjWithMtl(fish, [paloma.position.x, paloma.position.y, paloma.position.z], [paloma.rotation.x, 1.5, 0], 0.3, fishes, 1);
    //console.log(bolsaBolillos);

}

//Function that will load a chicken with a knife in its planned position as well as it's needed rotation in the y-axis.
//@paloma - receives the model that will be loaded, in this case the chicken with a knife
//@rotation - receives the rotation in the y-axis that will make sure the pidgeon is looking in the right way.
function shootKevin(paloma, rotation) {

    loadObjWithMtl(enemyModels[0], [paloma.position.x, paloma.position.y, paloma.position.z], [paloma.rotation.x, rotation, paloma.rotation.z], 2, kevins, 1);
    //console.log(bolsaBolillos);

}

//Function that will load the dinosaur enemy model
function spawnDino() {
    loadObjWithMtl(enemyModels[2], [0, 15, 0], [Math.PI / 2, 0, 0], 0.5, dinos, 0, 10);
}

//Function in charge of loading the Mario projectile that will be shot from the dinosaur enemy-type.
//@paloma - receives the Mario bullet that will be loaded when the dinosaur enemy fires.
function shootMario(paloma) {
    let mario = {
        modelo: marioObj,
        textura: marioMtl
    }
    loadObjWithMtl(mario, [paloma.position.x, paloma.position.y, paloma.position.z], [2.7, 0, 0], 0.03, bolsaBolillos, 1);

}
