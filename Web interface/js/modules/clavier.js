console.log("clavier.js chargé");

const symboles = {
    copyright: "https://static.wikia.nocookie.net/ktane/images/0/0b/Keypad01.png",
    filledstar: "https://static.wikia.nocookie.net/ktane/images/f/f9/Keypad02.png/revision/latest?cb=20200602190650",
    smileyface: "https://static.wikia.nocookie.net/ktane/images/1/16/Keypad04.png/revision/latest?cb=20200602190650",
    hollowstar: "https://static.wikia.nocookie.net/ktane/images/3/3d/Keypad03.png/revision/latest?cb=20200602190650",
    doublek: "https://static.wikia.nocookie.net/ktane/images/6/67/Keypad05.png/revision/latest?cb=20200602190651",
    omega: "https://static.wikia.nocookie.net/ktane/images/2/27/Keypad06.png/revision/latest?cb=20200602190651",
    squidknife: "https://static.wikia.nocookie.net/ktane/images/e/e4/Keypad07.png/revision/latest?cb=20200602190652",
    pumpkin: "https://static.wikia.nocookie.net/ktane/images/9/9e/Keypad08.png/revision/latest?cb=20200602190652",
    hookn: "https://static.wikia.nocookie.net/ktane/images/9/9d/Keypad09.png/revision/latest?cb=20200602190653",
    teepee: "https://static.wikia.nocookie.net/ktane/images/f/f4/Keypad10.png/revision/latest?cb=20200602190653",
    six: "https://static.wikia.nocookie.net/ktane/images/0/03/Keypad11.png/revision/latest?cb=20200602190654",
    squigglyn: "https://static.wikia.nocookie.net/ktane/images/5/50/Keypad12.png/revision/latest?cb=20200602190654",
    at: "https://static.wikia.nocookie.net/ktane/images/b/b7/Keypad13.png/revision/latest?cb=20200602190655",
    ae: "https://static.wikia.nocookie.net/ktane/images/f/f0/Keypad14.png/revision/latest?cb=20200602190655",
    meltedthree: "https://static.wikia.nocookie.net/ktane/images/3/30/Keypad15.png/revision/latest?cb=20200602190655",
    euro: "https://static.wikia.nocookie.net/ktane/images/6/6b/Keypad16.png/revision/latest?cb=20200602190656",
    circle: "https://static.wikia.nocookie.net/ktane/images/9/91/Keypad17.png/revision/latest?cb=20200602190656",
    nwithhat: "https://static.wikia.nocookie.net/ktane/images/d/d4/Keypad18.png/revision/latest?cb=20200602190657",
    dragon: "https://static.wikia.nocookie.net/ktane/images/3/32/Keypad19.png/revision/latest?cb=20200602190657",
    questionmark: "https://static.wikia.nocookie.net/ktane/images/3/34/Keypad20.png/revision/latest?cb=20200602190658",
    paragraph: "https://static.wikia.nocookie.net/ktane/images/7/7d/Keypad21.png/revision/latest?cb=20200602191353",
    rightc: "https://static.wikia.nocookie.net/ktane/images/2/28/Keypad22.png/revision/latest?cb=20200602191353",
    leftc: "https://static.wikia.nocookie.net/ktane/images/d/d8/Keypad23.png/revision/latest?cb=20200602191355",
    pitchfork: "https://static.wikia.nocookie.net/ktane/images/5/5c/Keypad24.png/revision/latest?cb=20200602191356",
    tripod: "https://static.wikia.nocookie.net/ktane/images/a/aa/Keypad25.png/revision/latest?cb=20200602191356",
    cursive: "https://static.wikia.nocookie.net/ktane/images/b/b3/Keypad26.png/revision/latest?cb=20200602191357",
    tracks: "https://static.wikia.nocookie.net/ktane/images/7/70/Keypad27.png/revision/latest?cb=20200602191357",
    balloon: "https://static.wikia.nocookie.net/ktane/images/0/06/Keypad28.png/revision/latest?cb=20200602191357",
    weirdnose: "https://static.wikia.nocookie.net/ktane/images/b/b0/Keypad29.png/revision/latest?cb=20200602191358",
    upsidedowny: "https://static.wikia.nocookie.net/ktane/images/1/16/Keypad30.png/revision/latest?cb=20200602191358",
    bt: "https://static.wikia.nocookie.net/ktane/images/2/27/Keypad31.png/revision/latest?cb=20200602191359",
}

/**
     * @typedef {"copyright"|"filledstar"|"smileyface"|"hollowstar"|"doublek"|"omega"|"squidknife"|"pumpkin"|"hookn"|"teepee"|"six"|"squigglyn"|"at"|"ae"|"meltedthree"|"euro"|"circle"|"nwithhat"|"dragon"|"questionmark"|"paragraph"|"rightc"|"leftc"|"pitchfork"|"tripod"|"cursive"|"tracks"|"balloon"|"weirdnose"|"upsidedowny"|"bt"} Clavier_Symbole
     */
    /** @type {Array.<Array.<Clavier_Symbole>>} */
    let manuel_clavier = [
        ["balloon", "at", "upsidedowny", "squigglyn", "squidknife", "hookn", "leftc"],
        ["euro", "balloon", "leftc", "cursive", "hollowstar", "hookn", "questionmark"],
        ["copyright", "pumpkin", "cursive", "doublek", "meltedthree", "upsidedowny", "hollowstar"],
        ["six", "paragraph", "bt", "squidknife", "doublek", "questionmark", "smileyface"],
        ["pitchfork", "smileyface", "bt", "rightc", "paragraph", "dragon", "filledstar"],
        ["six", "euro", "tracks", "ae", "pitchfork", "nwithhat", "omega"]
    ]



class Clavier extends Module{
    static id=2;
    id=2;
    constructor(){
        super();
        this.solutionGenerated=false;
        this.symbolesChoisis=[];
        this.symboles=[];
        this.solution=[];
        this.svg=null;
    }

    

    
    /**
     * @typedef {"copyright"|"filledstar"|"smileyface"|"hollowstar"|"doublek"|"omega"|"squidknife"|"pumpkin"|"hookn"|"teepee"|"six"|"squigglyn"|"at"|"ae"|"meltedthree"|"euro"|"circle"|"nwithhat"|"dragon"|"questionmark"|"paragraph"|"rightc"|"leftc"|"pitchfork"|"tripod"|"cursive"|"tracks"|"balloon"|"weirdnose"|"upsidedowny"|"bt"} Clavier_Symbole
     */
    /** @type {Array.<Array.<Clavier_Symbole>>} */
    


    // Generate the SVG
    

    appendSVG(){
        $(() => {
            $("body").append(svg);
        })
    }


     getDefaultSvg() {
        return `
        <svg width="400" height="400" xmlns="http://www.w3.org/2000/svg">
                    
            <!-- Module -->
            <rect x="0" y="0" width="400" height="400" fill="#545454" stroke="black" stroke-width="2" />

            <!-- Led verte en haut à droite -->
            <circle cx="350" cy="50" r="10" stroke="black" stroke-width="2" fill="green" />

            <!-- 4 boutons -->
            <rect x="100" y="100" width="84" height="84" fill="#6d6e70" />
            
            <rect x="216" y="100" width="84" height="84" fill="#6d6e70" />
            
            <rect x="100" y="216" width="84" height="84" fill="#6d6e70" />
            
            <rect x="216" y="216" width="84" height="84" fill="#6d6e70" />
        
        </svg> 
        `
    }
    test(){
        console.log("fonction getDefaultSvg terminee");
    }
    updateSvgWithSolution(){
        console.log("test");
        let svg=$(`svg[data-id=${this.id}]`);
        
        svg.off("click");

        console.log("Symboles choisis :")
        console.log(this.symbolesChoisis);
        console.log(this.solution[0]);
        configureModule(this.id,...this.solution);// this.solution prend des entiers(données à envoyer à l'arduino), à cause d'un problème inconnu
                                                    //la première valeur du tableau solution est supprimée donc on rajoute une valeur inutile afin que 
                                                    //tout le tableau soit pris en compte
        //console.log(symboles[this.symbolesChoisis[0]]);
        let str = `
        <svg width="400" height="400" xmlns="http://www.w3.org/2000/svg">
                    
            <!-- Module -->
            <rect x="0" y="0" width="400" height="400" fill="#545454" stroke="black" stroke-width="2" />

            <!-- Led verte en haut à droite -->
            <circle cx="350" cy="50" r="10" stroke="black" stroke-width="2" fill="green" />

            <!-- 4 boutons -->
            <rect x="100" y="100" width="84" height="84" fill="#6d6e70" />
            <image x="110" y="110" width="64" href="${symboles[this.symbolesChoisis[0]]}" />
            
            <rect x="216" y="100" width="84" height="84" fill="#6d6e70" />
            <image x="226" y="110" width="64" href="${symboles[this.symbolesChoisis[1]]}" />
            
            <rect x="100" y="216" width="84" height="84" fill="#6d6e70" />
            <image x="110" y="226" width="64" href="${symboles[this.symbolesChoisis[2]]}" />
            
            <rect x="216" y="216" width="84" height="84" fill="#6d6e70" />
            <image x="226" y="226" width="64" href="${symboles[this.symbolesChoisis[3]]}" />
        
        </svg> 
        `
        
        svg.html(str);

        svg.css("cursor", "default");

        svg.html(svg.html());
        
        
    }

    generateSolution(){
        var i=0;
        var j=0;
        this.symboles=[].concat(this.symbolesChoisis);//permet de "cloner le tableau"
        // Take a random array from the manuel
        let clavier = manuel_clavier[Math.floor(Math.random() * manuel_clavier.length)];
        // Take 4 unique symbols from the array
        while(this.symbolesChoisis.length < 4) {
            let symbole = clavier[Math.floor(Math.random() * clavier.length)];
            if(!this.symbolesChoisis.includes(symbole)) this.symbolesChoisis.push(symbole);
        }
        //returns the order of the symboles
        for(i=0;i<clavier.length;i++){
            for(j=0;j<this.symbolesChoisis.length;j++){
                if(clavier[i]==this.symbolesChoisis[j]){
                    this.solution.push(j+1);
                }
            }
        }
        console.log(this.solution);
        this.symboles=[].concat(this.symbolesChoisis);//permet de "cloner le tableau"        console.log("blablabla");
        console.log("Symboles choisis dans generate solution ")
        console.log(this.symbolesChoisis);
        this.solutionGenerated=true;
        this.updateSvgWithSolution();
    }
}

addModuleAvailable(Clavier);

//régler le problème d'affichage