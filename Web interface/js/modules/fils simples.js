console.log("fils simples.js chargé");

class Fils_Simples extends Module {

    static id=1;
    id=1;

    constructor() {
        super();
        this.solutionGenerated = false;
        this.nbFils = null;
        this.filsCouleurs = [];
    }
    
    getDefaultSvg() {
        return `
        <svg width="400" height="400" xmlns="http://www.w3.org/2000/svg">
                
                <!-- Module -->
                <rect x="0" y="0" width="400" height="400" fill="#545454" stroke="black" stroke-width="2" />
        
                <!-- Emplacements des 6 fils à gauche et à droite  -->
                <rect x="60" y="100" width="10" height="10" fill="black" />
                <rect x="60" y="150" width="10" height="10" fill="black" />
                <rect x="60" y="200" width="10" height="10" fill="black" />
                <rect x="60" y="250" width="10" height="10" fill="black" />
                <rect x="60" y="300" width="10" height="10" fill="black" />
                <rect x="60" y="350" width="10" height="10" fill="black" />
                 
                <rect x="330" y="100" width="10" height="10" fill="black" />
                <rect x="330" y="150" width="10" height="10" fill="black" />
                <rect x="330" y="200" width="10" height="10" fill="black" />
                <rect x="330" y="250" width="10" height="10" fill="black" />
                <rect x="330" y="300" width="10" height="10" fill="black" />
                <rect x="330" y="350" width="10" height="10" fill="black" />
        
                <!-- Led verte en haut à droite -->
                <circle cx="350" cy="50" r="10" stroke="black" stroke-width="2" fill="green" />
                
        
              </svg>`;
    }

    /**
     * @description Renvoie le fil à couper entre 1 et 6
     * @param {Number} nbFils 
     * @param {Array.<"rouge"|"blanc"|"bleu"|"jaune"|"noir">} couleurs 
     * TODO numéro de série
     */
    getWireToCut(nbFils, couleurs) {
        if(nbFils == 3) {
            if(!couleurs.includes("rouge")) return 2;
            else if (couleurs[2] == "blanc") return 3;
            else if (couleurs.filter(couleur => couleur == "bleu").length > 1) return couleurs.lastIndexOf("bleu") + 1;
            else return 3;
        } else if (nbFils == 4) {
            if(couleurs.filter(couleur => couleur == "rouge").length > 1 && dernierChiffreImpair()) return couleurs.lastIndexOf("rouge") + 1;
            else if (couleurs[3] == "jaune" && !couleurs.includes("rouge")) return 1;
            else if (couleurs.filter(couleur => couleur == "bleu").length == 1) return 1;
            else if (couleurs.filter(couleur => couleur == "jaune").length > 1) return 4;
            else return 2;
        } else if (nbFils == 5) {
            if(couleurs[4] == "noir" && dernierChiffreImpair()) return 4;
            else if (couleurs.filter(couleur => couleur == "rouge").length == 1 && couleurs.filter(couleur => couleur == "jaune").length > 1) return 1;
            else if (!couleurs.includes("noir")) return 2;
            else return 1;
        } else if (nbFils == 6) {
            if(!couleurs.includes("jaune") && dernierChiffreImpair()) return 3;
            else if (couleurs.filter(couleur => couleur == "jaune").length == 1 && couleurs.filter(couleur => couleur == "blanc").length > 1) return 4;
            else if (!couleurs.includes("rouge")) return 6;
            else return 4;
        }
    }

    generateSolution() {

        if(this.solutionGenerated) return;

        const couleursPossibles = ["rouge", "blanc", "bleu", "jaune", "noir"];

        // Générer un nombre de fils entre 3 et 6
        this.nbFils = Math.floor(Math.random() * 4) + 3;
        
        // Générer nbFils de couleurs "rouge", "blanc", "bleu", "jaune", "noir", 
        for(let i=0; i<this.nbFils; i++) {
            this.filsCouleurs.push(couleursPossibles[Math.floor(Math.random() * couleursPossibles.length)]);
        }

        console.log(this.filsCouleurs);

        this.solutionGenerated = true;

        // Chercher la solution
        this.solution = this.getWireToCut(this.nbFils, this.filsCouleurs);   
        
        console.log(`Solution : ${this.solution}`);

        this.updateSvgWithSolution();
    }

    /**
     * @description Met à jour le SVG avec la solution
     * @returns {String}
     */
    updateSvgWithSolution() {

        let svg = $(`svg[data-id=${this.id}]`);
        var couleurActuelle
        const positionsYFils = [100, 150, 200, 250, 300, 350];
        // Ajouter les fils :
        for(let i=0; i<this.nbFils; i++) {



            switch(this.filsCouleurs[i]) {
                case "rouge":
                    couleurActuelle = "red";
                    break;
                case "blanc":
                    couleurActuelle = "white";
                    break;
                case "bleu":
                    couleurActuelle = "blue";
                    break;
                case "jaune":
                    couleurActuelle = "yellow";
                    break;
                case "noir":
                    couleurActuelle = "black";
                    break;
            }

            svg.append(`<line x1="65" y1="${positionsYFils[i]+5}" x2="335" y2="${positionsYFils[i]+5}" stroke="${couleurActuelle}" stroke-width="5" />`);
        }

        svg.css("cursor", "default");

        svg.off("click");

        refreshModulesDiv();

    }
}

addModuleAvailable(Fils_Simples);