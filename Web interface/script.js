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

    console.log(string);

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
                    reader.read().then(processText);
                });
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