let renderer = null, 
scene = null, 
camera = null,
root = null,
group = [],
sphere = null,
sphereTextured = null;
let pivotPlanetas = [],
    planetas = [],
    numLunas = [0,0,0,1,2,10,10,10,10,5],
    sunPivots = [],
    moonPivots = [];

let materialName = "phong-textured";	
let textureOn = true;

let duration = 10000; // ms
let currentTime = Date.now();

let materials = {};
let mapUrl = "./images/moon_1024.jpg";
let textureMap = null;

// Unlit (Basic Material) - With this material type, only the textures, colors, and transparency values are used to render the surface of the object. There is no contribution from lights in the scene.

// Phong shading - This material type implements a simple, fairly realistic-looking shading model with high performance. Phong-shaded objects will show brightly lit areas (specular reflections) where light hits directly, will light well along any edges that mostly face the light source, and will darkly shade areas where the edge of the object faces away from the light source.

// Lambertian reflectance - In Lambert shading, the apparent brightness of the surface to an observer is the same regardless of the observer’s angle of view. 
function createMaterials(mapUrl)
{
    // Create a textre phong material for the cube
    // First, create the texture map
    textureMap = new THREE.TextureLoader().load(mapUrl);

    materials["basic"] = new THREE.MeshBasicMaterial();
    materials["phong"] = new THREE.MeshPhongMaterial();
    materials["lambert"] = new THREE.MeshLambertMaterial();
    materials["basic-textured"] = new THREE.MeshBasicMaterial({ map: textureMap });
    materials["phong-textured"] = new THREE.MeshPhongMaterial({ map: textureMap });
    materials["lambert-textured"] = new THREE.MeshLambertMaterial({ map: textureMap });
}

//Definir la clase de mis planetas
class Planeta{
    constructor(pivot, parentPivot, geometry, position, textureUrl, bumpUrl, material, rotation, mesh){
        this.pivot = new THREE.Object3D();
        this.parentPivot = parentPivot;
        this.geometry = geometry;
        this.position = position;
        this.textureUrl = textureUrl;
        this.bumpUrl = bumpUrl;
        this.material = material;
        this.rotation = rotation;
        this.mesh = mesh;
    }

    Inicializar(option){
        //Anadir pivote al pivote padre
        this.parentPivot.add(this.pivot);
        //Declarar posicion
        this.pivot.position.set(this.position[0],this.position[1],this.position[2]);
        //Crear la textura
        createMaterials(this.textureUrl);
        //Obtener el mesh
        this.mesh = createPlanet(this.geometry, this.material, this.textureUrl, this.bumpUrl);
        //Anadir el mesh al pivote
        this.pivot.add(this.mesh);
        if(option == 0){
            //A;adir los planetas a la lista
            pivotPlanetas.push(this.pivot);
            planetas.push(this);
        }

    }
}

function animate() 
{
    let now = Date.now();
    let deltat = now - currentTime;
    currentTime = now;
    let fract = deltat / duration;
    let angle = 0.01;

    // Rotate the sphere group about its Y axis

    pivotPlanetas.forEach(p => {
        p.rotation.y += angle;
    });

    sunPivots.forEach(s => {
        angle -= 0.001;
        s.rotation.y += angle;
        //console.log(angle);
    })


    angle = Math.PI * 1 * fract;
    moonPivots.forEach(m => {
        angle += Math.floor(Math.random()*10)*0.0001;
        m.rotation.y += angle;
    })

    //console.log("finish");

}

function run()
{
    requestAnimationFrame( () => run());
    
    // Render the scene
    renderer.render( scene, camera );
    controls.update();

    // Spin the cube for next frame
    animate();
}


// Changes the diffuse color of the material. The material’s diffuse color specifies how much the object reflects lighting sources that cast rays in a direction — directional, point, and spotlights.
function setMaterialDiffuse(r, g, b)
{
    r /= 255;
    g /= 255;
    b /= 255;
    
    materials["basic"].color.setRGB(r, g, b);
    materials["phong"].color.setRGB(r, g, b);
    materials["lambert"].color.setRGB(r, g, b);
    materials["basic-textured"].color.setRGB(r, g, b);
    materials["phong-textured"].color.setRGB(r, g, b);
    materials["lambert-textured"].color.setRGB(r, g, b);
}

// The specular color combines with scene lights to create reflected highlights from any of the object's vertices facing toward light sources.
function setMaterialSpecular(r, g, b)
{
    r /= 255;
    g /= 255;
    b /= 255;
    
    materials["phong"].specular.setRGB(r, g, b);
    materials["phong-textured"].specular.setRGB(r, g, b);
}

function setMaterial(name)
{
    materialName = name;
    if (textureOn)
    {
        sphere.visible = false;
        sphereTextured.visible = true;
        sphereTextured.material = materials[name];
    }
    else
    {
        sphere.visible = true;
        sphereTextured.visible = false;
        sphere.material = materials[name];
    }
}


//Funcion para generar el mesh del planeta
function createPlanet(geometryArr, type, texture, bump){
    // Create the sphere geometry
    let geometry = new THREE.SphereGeometry(geometryArr[0], geometryArr[1], geometryArr[2]);
    
    
    // And put the geometry and material together into a mesh
    sphere = new THREE.Mesh(geometry, materials[type]);
    sphere.visible = false;

    // And put the geometry and material together into a mesh
    if(bump != 0){
        let textureMap = new THREE.TextureLoader().load(texture);
        let bumpMap = new THREE.TextureLoader().load(bump);

        materials[type] = new THREE.MeshPhongMaterial({ bumpMap: bumpMap, bumpScale: 0.06});
        materials[type+"-textured"] = new THREE.MeshPhongMaterial({ map: textureMap, bumpMap: bumpMap, bumpScale: 0.06 });
    }


    sphereTextured = new THREE.Mesh(geometry, materials[type+"-textured"]);
    sphereTextured.visible = true;
    setMaterial(type+"-textured");

    return sphereTextured;
}
function createSaturnRing(type){
        // Create the sphere geometry
        let geometry = new THREE.RingBufferGeometry( 5, 3, 60);
        /*
            Funcion para poner la textura obtenida de:
            https://discourse.threejs.org/t/applying-a-texture-to-a-ringgeometry/9990
        */
        var pos = geometry.attributes.position;
        var v3 = new THREE.Vector3();
        for (let i = 0; i < pos.count; i++){
            v3.fromBufferAttribute(pos, i);
            geometry.attributes.uv.setXY(i, v3.length() < 4 ? 0 : 1, 1);
        }   
    
        // And put the geometry and material together into a mesh
        sphere = new THREE.Mesh(geometry, materials[type]);
        sphere.visible = false;
    
        // And put the geometry and material together into a mesh
        sphereTextured = new THREE.Mesh(geometry, materials[type+"-textured"]);
        sphereTextured.visible = true;
        setMaterial(type+"-textured");
    
        return sphereTextured;
}

function crearLunas(){
    planetas.forEach(e => {
        //console.log(numLunas[planetas.indexOf(e)]);
        for(let i = 0; i < numLunas[planetas.indexOf(e)];i++){
            let pivot = new THREE.Object3D();
            moonPivots.push(pivot);
            e.pivot.add(pivot);
            let moon = new Planeta(0, pivot,[0.5,20,20],[4,(Math.floor(Math.random() *5)-2),0],"./images/moon_1024.jpg","./images/moon_bump.jpg","phong");
            moon.Inicializar(0);
        }
    });

}

function crearCinturonAsteroides(sunPivot,r){

    
    for(let i = 0; i < 80;i++){
        let angle = Math.random()*Math.PI*2;
        let radius = Math.floor(Math.random() * (r - (r-6)))+(r-6);
        let x = Math.cos(angle)*radius;
        let z = Math.sin(angle)*radius;
        //Obtener un angulo aleatorio para la posicion inicial
        let asteroid = new Planeta(0, sunPivot,[Math.floor(Math.random() *10)*0.1,20,20],[x,0,z],"./images/moon_1024.jpg","./images/moon_bump.jpg","phong");
        asteroid.Inicializar(1);
    }

}

function createScene(canvas) 
{    
    // Create the Three.js renderer and attach it to our canvas
    renderer = new THREE.WebGLRenderer( { canvas: canvas, antialias: true } );

    // Set the viewport size
    renderer.setSize(canvas.width, canvas.height);

    // Create a new Three.js scene
    scene = new THREE.Scene();

    scene.background = new THREE.TextureLoader().load("./images/galaxy-background-blue-lovely-blue-galaxy-wallpaper-wallpapersafari-this-month-of-galaxy-background-blue.jpg");//new THREE.Color( 0.2, 0.2, 0.2 );

    // Add  a camera so we can view the scene
    camera = new THREE.PerspectiveCamera( 45, canvas.width / canvas.height, 1, 4000 );
    camera.position.z = 10;
    scene.add(camera);
    
    // Create a group to hold all the objects
    root = new THREE.Object3D;
    
    var light = new THREE.PointLight( 0xffffff, 1, 500 );
    light.position.set( 0, 0, 0 );
    root.add(light);

    //Crear el sol
    let Sun = new Planeta(0,root,[5,20,20],[0,0,0],"./images/2k_sun.jpg",0,"basic");
    Sun.Inicializar(0);

    for(let i =0; i < 9; i++){
        let pivot = new THREE.Object3D();
        sunPivots.push(pivot);
        root.add(pivot);
    }

    //Crear mis planetas
    let mercury = new Planeta(0,sunPivots[0], [1,20,20],[10,0,0],"./images/mercurymap.jpg","./images/mercurybump.jpg","phong");
    mercury.Inicializar(0);
    let venus = new Planeta(0,sunPivots[1], [2,20,20],[20,0,0],"./images/venusmap.jpg","./images/venusmap.jpg","phong");
    venus.Inicializar(0);
    let earth = new Planeta(0,sunPivots[2], [2,20,20],[30,0,0],"./images/earth_atmos_2048.jpg","./images/venusmap.jpg","phong");
    earth.Inicializar(0);
    let mars = new Planeta(0,sunPivots[3], [1.3,20,20],[40,0,0],"./images/mars_1k_color.jpg","./images/marsbump1k.jpg","phong");
    mars.Inicializar(0);
    crearCinturonAsteroides(sunPivots[3],60);
    let jupiter = new Planeta(0,sunPivots[4], [3,20,20],[70,0,0],"./images/2k_jupiter.jpg",0,"phong");
    jupiter.Inicializar(0);
    let saturn = new Planeta(0,sunPivots[5], [2.8,20,20],[80,0,0],"./images/2k_saturn.jpg",0,"phong");
    saturn.Inicializar(0);
    let uranus = new Planeta(0,sunPivots[6], [2.4,20,20],[90,0,0],"./images/2k_uranus.jpg",0,"phong");
    uranus.Inicializar(0);
    let neptune = new Planeta(0,sunPivots[7], [2.4,20,20],[100,0,0],"./images/2k_neptune.jpg",0,"phong");
    neptune.Inicializar(0);
    let pluto = new Planeta(0,sunPivots[8], [1,20,20],[110,0,0],"./images/plutomap1k.jpg","./images/plutobump1k.jpg","phong");
    pluto.Inicializar(0);

    //Crear los anillos
    planetas.forEach(element => {
        let geometry = new THREE.RingGeometry( (element.position[0]+0.15), (element.position[0]+0.2), 60);
        let material = new THREE.MeshBasicMaterial( { color: 0xffff00, side: THREE.DoubleSide } );
        let mesh = new THREE.Mesh( geometry, material );
        mesh.rotation.x = Math.PI / 2;
        Sun.pivot.add(mesh);
    });

    //Crear anillo para Saturno
    createMaterials("./images/2k_saturn_ring_alpha.png");
    let saturnRing = createSaturnRing("basic");
    saturnRing.rotation.x = Math.PI / 2.5;
    saturn.pivot.add(saturnRing);

    //Crear lunas
    crearLunas();

    scene.add( root );

    // add mouse handling so we can rotate the scene
    //addMouseHandler(canvas, root);
}
    