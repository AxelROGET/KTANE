var buffer = [];

var writer;

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

/**
 * @param {Array.<Number>} data
 */
function traitementBuffer(data) {

    var string = data.map(value => String.fromCharCode(value)).join("");

    // console.log(string);

    try {
        var json = JSON.parse(string);
        console.log(json);

        if(json.modules) {
            Object.keys(json.modules).forEach(id => {
                
                if(json.modules[id] == '-1' && getModuleConnected(id)) {
                    removeModuleConnected(id);

                    // Supprimer le SVG
                    $(`svg[data-id=${id}]`).remove();
                }

                else if(json.modules[id] != '-1' && !getModuleConnected(id) && getModuleAvailable(id)) {
                    addModuleConnected(new (getModuleAvailable(id))());

                    // Ajouter le SVG par défaut :
                    let svg = $(getModuleConnected(id).getDefaultSvg()); 
                    // Append le SVG avec en data-id l'id du module
                    svg.attr("data-id", id)
                    .on("click", function() {
                        // Configurer le module
                        getModuleConnected(id).generateSolution();
                    })
                    .css("cursor", "pointer")
                    .appendTo("#modules");
                    
                }
            })
        }
    } catch {
        console.error("Erreur lors du traitement du buffer");
        console.log(string);
    }

}

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
                reader.read().then(function processText({ done, value }) {
                    appendBuffer(value);
                    reader.read().then(processText).catch((error) => {
                        reader.read().then(processText);
                    })
                }).catch((error) => {
                    reader.read().then(processText);
                })
                writer = port.writable.getWriter();
            });
        });
    }
}

$(function () {
    $("<button>").text("Se connecter").on("click", function () {
        requestSerial(this);
    }).appendTo("body");
});