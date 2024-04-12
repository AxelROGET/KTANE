console.log("Morse.js chargé");


class Morse extends Module{
    static id=3;
    id=3;
    constructor(){
        super();
        this.solutionGenerated=false;
        this.solution=null;
    }

    getDefaultSvg() {
        console.log("flagMorse");
        let coordonnees_x_svg = 150;
        let coordonnees_y_svg = 250;
        let quatre_sept_segment = "";
        quatre_sept_segment += `
        <rect x="${coordonnees_x_svg+2}" y="${coordonnees_y_svg}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+19}" y="${coordonnees_y_svg + 2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg + 19}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2}" y="${coordonnees_y_svg + 38}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg}" y="${coordonnees_y_svg +2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2}" y="${coordonnees_y_svg + 19}" width="17" height="2" fill="#F00" />
        `;
        quatre_sept_segment += `
        <rect x="${coordonnees_x_svg+2+25}" y="${coordonnees_y_svg}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+19+25}" y="${coordonnees_y_svg + 2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg + 19+25}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2+25}" y="${coordonnees_y_svg + 38}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+25}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+25}" y="${coordonnees_y_svg +2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2+25}" y="${coordonnees_y_svg + 19}" width="17" height="2" fill="#F00" />
        `;
        quatre_sept_segment += `
        <rect x="${coordonnees_x_svg+2+25+25}" y="${coordonnees_y_svg}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+19+25+25}" y="${coordonnees_y_svg + 2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg + 19+25+25}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2+25+25}" y="${coordonnees_y_svg + 38}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+25+25}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+25+25}" y="${coordonnees_y_svg +2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2+25+25}" y="${coordonnees_y_svg + 19}" width="17" height="2" fill="#F00" />
        `;
        quatre_sept_segment += `
        <rect x="${coordonnees_x_svg+2+25+25+25}" y="${coordonnees_y_svg}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+19+25+25+25}" y="${coordonnees_y_svg + 2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg + 19+25+25+25}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2+25+25+25}" y="${coordonnees_y_svg + 38}" width="17" height="2" fill="#F00" />
        <rect x="${coordonnees_x_svg+25+25+25}" y="${coordonnees_y_svg + 21}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+25+25+25}" y="${coordonnees_y_svg +2}" width="2" height="17" fill="#F00" />
        <rect x="${coordonnees_x_svg+2+25+25+25}" y="${coordonnees_y_svg + 19}" width="17" height="2" fill="#F00" />
        `;
        // document.getElementById("1").innerHTML = quatre_sept_segment;
        return `
        <svg width="400" height="400" xmlns="http://www.w3.org/2000/svg">
                    
        <!-- Module -->
        <rect x="0" y="0" width="400" height="400" fill="#545454" stroke="black" stroke-width="2" />
    
        <!-- Led verte en haut à droite -->
        <circle cx="350" cy="50" r="10" stroke="black" stroke-width="2" fill="green" />
    
        <!--led clignotante-->
        <circle cx="200" cy="150" r="10" fill="#FFFF3D"/>
        <!--triangle de droite-->
        <svg height="300" width="400" xmlns="http://www.w3.org/2000/svg">
            <polygon points="300,180 325,210 300,240" style="fill:grey;" />
        </svg>
        <!--triangle de gauche-->
        <svg height="300" width="400" xmlns="http://www.w3.org/2000/svg">
            <polygon points="100,180 75,210 100,240" style="fill:grey;" />
        </svg>
        <!--bouton tx -->
        <rect x="125" y="300" width="150" height="42" fill="#6d6e70" />
        <text id="TX" x="190" y="325">TX</text>
        <svg xmlns="http://www.w3.org/2000/svg" width="300" height="1500">${quatre_sept_segment}</svg>
    
        </svg> 
        `
    }

}

addModuleAvailable(Morse);

