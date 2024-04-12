console.log("module.js chargé");

class Module {

    /** @type {Number} */
    static id;
    id;

    constructor() {
        if(this.constructor == Module) {
            throw new Error("Module can't be instanciated");
        }
    }

    /**
     * @description Renvoie le SVG par défaut du module
     * @returns {String}
     */
    getDefaultSvg() {
        
    }

    generateSolution() {

    }

    updateSvgWithSolution() {

    }
}


/**
 * ! Modules disponibles
 */
/** @type {Module[]} */
var modulesAvailable = [];

function addModuleAvailable(module) {
    modulesAvailable.push(module);
}

function getModuleAvailable(id) {
    return modulesAvailable.find(module => module.id == id);
}

/**
 * ! Modules connectés
 */
/** @type {Module[]} */
var modulesConnected = [];

function addModuleConnected(module) {
    modulesConnected.push(module);
}

function getModuleConnected(id) {
    return modulesConnected.find(module => module.id == id);
}

function getModulesConnected() {
    return modulesConnected;
}

function removeModuleConnected(id) {
    modulesConnected = modulesConnected.filter(module => module.id != id);
}

/**
 * ! Refresh modules div
 */
function refreshModulesDiv() {
    // $("#modules").html($("#modules").html())
    $("#modules").clone(true).appendTo("#modules");
}

function configureModule(addr, ...data) {
    console.log(`Configuring module ${addr} with data : ${data}`);
    sendSerial(0, addr, ...data);
}