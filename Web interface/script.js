var buffer = [];

var globalWriter;

/**
 * @param {Array.<Number>} data
 */
function traitementBuffer(data) {

    var string = data.map(value => String.fromCharCode(value)).join("");

    console.log(string);

}

/**
 * 
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

function requestSerial() {
    // Ouvrir le port série 
    if (!navigator.serial) {
        alert("Votre navigateur ne supporte pas l'API Web Serial");
    } else {
        // Ouvrir le port série
        navigator.serial.requestPort().then(function (port) {
            // Ouvrir le port série
            port.open({ baudRate: 9600 }).then(function () {
                // Lire les données
                const reader = port.readable.getReader();
                reader.read().then(function processText({ done, value }) {
                    // Afficher les données
                    //console.log("Données reçues")
                    //console.log(value);
                    appendBuffer(value);
                    //console.log(value.toString());
                    // Récupérer les données suivantes
                    reader.read().then(processText);
                });
            });
        });
    }
}