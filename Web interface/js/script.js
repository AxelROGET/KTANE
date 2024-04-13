var buffer = [];

var writer;

const STATE_NON_CONNECTE = -1;
const STATE_RAS = 0;
const STATE_DESARME = 1;
const STATE_ERREUR = 2;
const STATE_ATTENTE_CONFIGURATION = 3;
const STATE_PRET = 4;

const SOUND = false;


/**
 * @param {Array.<Number>} data
 */
function traitementBuffer(data) {

    var string = data.map(value => String.fromCharCode(value)).join("");

    // console.log(string);

    try {
        var json = JSON.parse(string);
        console.log(json);

        // $("#debug").html(json.modules['1'])

        /**
         * ! ETATS DES MODULES
         */
        if(json.modules) {

            let ready = true;
            

            Object.keys(json.modules).forEach(id => {
                // TODO si un module est déconnecté reconnecté sans qu'il soit détecté, renvoyer sa configuration
                /**
                 ** Si le module a été déconnecté, on le supprime de la page
                 */
                if(json.modules[id] == STATE_NON_CONNECTE && getModuleConnected(id)) {
                    removeModuleConnected(id);

                    // Supprimer le SVG
                    $(`svg[data-id=${id}]`).remove();
                }

                /**
                 ** Si un nouveau module est connecté, on l'ajoute à la page
                 */
                else if(json.modules[id] != STATE_NON_CONNECTE && !getModuleConnected(id) && getModuleAvailable(id)) {
                    addModuleConnected(new (getModuleAvailable(id))());

                    // Ajouter le SVG par défaut :
                    let svg = $(getModuleConnected(id).getDefaultSvg()); 

                    console.log(svg);
                    // Append le SVG avec en data-id l'id du module
                    svg.attr("data-id", id)
                    .on("click", function() {
                        // Configurer le module
                        getModuleConnected(id).generateSolution();
                    })
                    .css("cursor", "pointer")
                    .appendTo("#modules");
                    
                }


                if(json.modules[id] == STATE_ATTENTE_CONFIGURATION) {
                    ready = false;
                }

            })


            if(ready) {
                $("#ready button").prop("disabled", false);
                $("#ready button").prop("title", "Lancer la partie");
                
            } else {
                $("#ready button").prop("disabled", true);
                $("#ready button").prop("title", "Veuillez configurer tous les modules pour pouvoir lancer la partie.");
            }


        }
    } catch {

        if(string == "Explosion") {
            soundExplosion();
        } else {
            console.error("Erreur lors de la lecture du buffer");
            console.log(string);
        }
    }

}

function soundExplosion() {
    if(!SOUND) return;
    var audio = new Audio("ressources/explosion.mp3");
    audio.play();
}

$(function () {
    $("<button>").text("Se connecter").on("click", function () {
        requestSerial(this);
    }).appendTo("body");

    $("#ready button").on("click", function() {
        sendSerial(0x01);
    })

    $("#config select[name=temps_jeu]").on("change", function() {
        console.log($(this).val());
        sendSerial(0,0,0,parseInt($(this).val())); 
    });

    $("#config select[name=max_erreurs]").on("change", function() {
        console.log($(this).val());
        sendSerial(0,0,1,parseInt($(this).val())); 
    });
});

/**
 * @param {ArrayBuffer} data 
 */
function appendBuffer(data) {
    data.forEach(value => {
        buffer.push(value);

        if (buffer[buffer.length - 1] == 10 && buffer[buffer.length - 2] == 13) {
            traitementBuffer(buffer.filter((value, index) => index < buffer.length - 2));
            buffer = [];
        }

    })
}

function requestSerial(button) {
    // Ouvrir le port série 
    if (!navigator.serial) {
        alert("Votre navigateur ne supporte pas l'API Web Serial");
    } else {
        
        // Ouvrir le port série
        const filter = { usbVendorId: 0x0403, usbProductId: 0x6001}
        navigator.serial.requestPort({filter}).then(function (port) {
            // Ouvrir le port série
            port.open({ baudRate: 9600 }).then(function () {
                $(button).hide();
                // Lire les données
                const reader = port.readable.getReader();

                function tryToReadBuffer() {
                    reader.read().then(function processText({ done, value }) {
                        appendBuffer(value);
                        tryToReadBuffer();
                    }).catch((error) => {

                        switch(error.stack) {
                            case "Error: Buffer overrun":
                                alert("Buffer overrun");
                                window.location.reload();
                                break;

                            case "Error: The device has been lost.":
                                alert("La bombe a été déconnectée.");
                                window.location.reload();
                                break;

                            default:
                                console.error("Une erreur s'est produite lors de la lecture du port série", error);
                                console.log(JSON.stringify(error, null, 2));
                                setTimeout(tryToReadBuffer, 500);
                                break;
                        }
                        
                    })
                }

                tryToReadBuffer();

                writer = port.writable.getWriter();
            });
        })
    }
}

/**
 * @param {Number} data
 * @description Envoie des données au port série
 */
function sendSerial(...bytes) {
    if (!writer) {
        console.error("Pas de port série ouvert.");
    }
    var data = new Uint8Array(bytes);
    writer.write(data).then(() => {
        console.log("Données envoyées");
    }).catch((error) => {
        console.error("Erreur lors de l'envoi des données", error);
    });

}