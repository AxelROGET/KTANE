const possibilités = ["ABCD-89"]
// TODO : ajouter les autres possibilités
// TODO affichage du numéro de série
var numéroDeSérie = "";
function generateNuméroDeSérie() {
    numéroDeSérie = possibilités[Math.floor(Math.random() * possibilités.length)];

    console.log(`Numéro de série : ${numéroDeSérie}`);
}

function dernierChiffreImpair() {

    if(numéroDeSérie == "") generateNuméroDeSérie();

    return numéroDeSérie[numéroDeSérie.length - 1] % 2 != 0;
}