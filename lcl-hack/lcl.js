
//SCRIPT PRINCIPAL
var page = require('webpage').create(),
  system = require('system'),
  fs = require('fs'),
  t, address;

if (system.args.length === 1) {
  console.log('Usage: lcl.js <identifiant> <code> (<agence> <numcompte> <lettrecle>)');
  phantom.exit();
}

phantom.outputEncoding = "utf-8";

console.log("Connexion à LCL...");

page.open('https://android.particuliers.secure.lcl.fr', function(status) {

  //Une fois le site chargé, on execute la fonction inputLogin
  waitforPageLoad(page,inputLogin);
  }

);

//FONCTIONS

//Permet d'attendre les chargements AJAX de manière pas trop immonde:
//On attend le loading de la page: timer de 2sec réinitialisé à chaque requête/truc reçu
function waitforPageLoad(page,callback)
  {

    waitForLoad = window.setTimeout(function() {
          callback(page);
        }, 2000);

    page.onResourceRequested = function(request) {
             window.clearTimeout(waitForLoad);
            //console.log('Request ' + JSON.stringify(request, undefined, 4))
            };

    page.onResourceReceived = function(response) {
              window.clearTimeout(waitForLoad);
              waitForLoad = window.setTimeout(function() {
                callback(page);
              }, 2000);


            };

  }

//Gestion du login à LCL
function inputLogin(page)
  {
    page.render('/var/www/pagelogin.png');
    console.log("Login à LCL...");

    //on hak lcl 
    //On remplit le form et on le submit via JS pour que toute la connerie de cryptage se fasse 
    page.evaluate(function(id,code) {

        document.getElementById("idUnique").value=id;
        document.getElementById("CodeId").value=code;
        
        document.getElementById("btnLoginPar").click();
      },system.args[1],system.args[2]);

      if (system.args.length === 6) {

        //Cas du détail d'un compte en particulier
        waitforPageLoad(page,getDoshDetail);

      }
      else
      {

        //Cas du listing de comptes
        waitforPageLoad(page,getAccounts);
          
      } 

    
  }

//Gestion de la récupération du listing de comptes
function getAccounts(page)
  {

    //si on est loggé
    console.log("Login réussi ! Récupération des comptes ...");
    //on peut maintenant récupérer la liste des comptes
    page.open('https://android.particuliers.secure.lcl.fr/outil/UWSP/Synthese?mobile=true', function (status) {

      waitforPageLoad(page,function(page){

       var jsonSource = page.plainText;
       var resultObject = JSON.parse(jsonSource);
       //console.log(page.plainText);

       //listing des comptes
       //structure: famille -> tableau de 4 types de comptes, pro, particulier, épargne, autres
       //chaque type de compte: libelle,listGroupe -> libelle/listCompte -> les Comptes
       for(var i in resultObject.famille) {
            
          var type = resultObject.famille[i];
          console.log("-----------");
          console.log(type.listGroupe[0].libelle + ":");

          for (var j in type.listGroupe[0].listCompte) {
            var compte = type.listGroupe[0].listCompte[j];
            console.log("---" + compte.natureLibelle + 
                        " --- CODE: " + compte.agence + "-" + compte.compte + compte.lettreCle + 
                        "  --- SOLDE: " + compte.soldeDisponible + " EUR");
          }

        }
        console.log("Le détail d'un compte peut être vu avec la commande <phantomjs lcl.js [ID] [CODE] [AGENCE] [NUMCOMPTE] [LETTRECLE]> ");

        phantom.exit();

      });

    });

  }


//Gestion de la récupération du détail d'un compte
//Assez similaire à getAccounts
function getDoshDetail(page)
  {
    agence = system.args[3];
    compte = system.args[4];
    lettre = system.args[5];
    
    //page.render('/var/www/pagelogged.png');
    //si on est loggé
    console.log("Login réussi ! Récupération du détail du compte précisé...");
    //on récupère le détail du compte spécifié
    page.open('https://android.particuliers.secure.lcl.fr/outil/UWLM/ListeMouvementsPar/accesListeMouvementsPar?agence='+agence+'&compte='+compte+lettre+'&lettreCle='+lettre+'&mobile=true', function (status) {

      waitforPageLoad(page,function(page){
    
        var jsonSource = page.plainText;
        var resultObject = JSON.parse(jsonSource);

        //Ecriture en rss et en format liveTile MS
        var pathRss = '/var/www/doshfeed.xml';
        var pathMS = '/var/www/doshtile.xml';
        var pathSMS = 'sms.txt';

        if (fs.exists(pathRss))
         fs.remove(pathRss);

         //détail du compte
         //structure: listeMvt - > tableau d'opérations -> objets json contenant libelle, montant, etc
         console.log(resultObject.compteCible.libelleTitulairePartie1 + " - " + resultObject.compteCible.natureLibelle);
         console.log("Solde Disponible: " + resultObject.compteCible.soldeDisponible + " EUR");
         console.log("Solde Comptable: " + resultObject.compteCible.soldeComptableSigne + " " + resultObject.compteCible.soldeComptable + " EUR");
         var soldeHistory = convertLCL(resultObject.compteCible.soldeComptableSigne+resultObject.compteCible.soldeComptable);  

         //w+ pour le rss/sms, w pour la livetile
         fs.write(pathRss, writeRSSHeader(resultObject.compteCible.natureLibelle + " : " + resultObject.compteCible.soldeDisponible + " EUR"), 'w+');

         var tileTxt11,tileTxt12,tileTxt2,tileTxt3;

         tileTxt11 = resultObject.compteCible.natureLibelle;
         tileTxt12 = resultObject.compteCible.soldeDisponible + " EUR";


         for(var i in resultObject.listeMvt) {

            var operation = resultObject.listeMvt[i];
            console.log("-----------");
            console.log(operation.libelleMouvement);


            date = operation.dateMouvement;
            //si c'est une opération non comptée par la banque, on ne fait pas le calcul du solde et on change l'affichage de la date
            //parce que les jsons de LCL c'est de la merde 
            if (operation.codeOperation === "CO ")
            {
              console.log(operation.montantMouvement + " " +operation.deviseMouvement);
            }
            else
            {
              date = date.slice(6,8) + "/" + date.slice(4,6) + "/" + date.slice(0, 4); 

              //calcul du solde précédent
              soldeHistory = soldeHistory - convertLCL(operation.montantCredit);
              soldeHistory = soldeHistory + convertLCL(operation.montantDebit);
            
             //console.log("+"+convertLCL(operation.montantCredit)+",-"+convertLCL(operation.montantDebit));

            console.log(operation.montantMouvement + " " +operation.deviseMouvement + " (Solde précédent : " + soldeHistory.toFixed(2) + " EUR)");
            }
            console.log(date);

            //écriture rss+tile
            fs.write(pathRss, writeRSSElement(operation.libelleMouvement+' : '+ operation.montantMouvement + " " +operation.deviseMouvement, "",date),  'w+');
            if (!tileTxt2)
              tileTxt2 = operation.libelleMouvement+' - '+ date + " : " +operation.montantMouvement + " " +operation.deviseMouvement;
            else
              if (!tileTxt3)
                tileTxt3 = operation.libelleMouvement+' - '+ date + " : " +operation.montantMouvement + " " +operation.deviseMouvement;

          }

          fs.write(pathMS, writeLiveTileXML(tileTxt11+"\r\n"+tileTxt12,tileTxt2,tileTxt3), 'w');
          fs.write(pathRss, "</channel></rss>", 'w+');
          fs.write(pathSMS, tileTxt11+" "+tileTxt12+"   "+tileTxt2+"   "+tileTxt3, 'w');

          phantom.exit();

      });

      
    });

  }
	
function convertLCL(number)
{
  res = parseFloat(number.replace(",","."));
  if (isNaN(res))
    return 0;
  else
    return res;
}

function writeRSSHeader(title)
{
  return '<?xml version="1.0" encoding="utf-8"?>'+
            '<rss version="2.0">'+
            '<channel>'+
            '<title>'+title+'</title>'+
            '<link>dosh.com</link>'+
            '<description>L O D S OF E M O N E</description>';
}

function writeRSSElement(name,date,description)
{
  return '<item>'+
           '<title>'+name+'</title>'+
           '<pubDate>'+date+'</pubDate>'+
           '<description>'+description+'</description>'+
          '</item>';

}

function writeLiveTileXML(text1,text2,text3)
{

  text21 = text2.split(":")[0];
  text22 = text2.split(":")[1];
  text31 = text3.split(":")[0];
  text32 = text3.split(":")[1];

  return '<tile>'+
            '<visual lang=\'en-US\' version=\'2\'>'+

              '<binding template="TileSquare150x150Text04"  contentId="0" branding="name">'+
              '<text id="1">'+text1+'</text>'+
              '</binding>'+

              '<binding template="TileWide310x150Text03"  contentId="0" branding="name">'+
              '<text id="1">'+text1+'</text>'+
              '</binding>'+

              '<binding template="TileSquare310x310BlockAndText01"  contentId="0" branding="name">'+
              '<text id="1">'+text1+'</text>'+
              //'<text id="2">'+text2+'</text>'+
              //'<text id="3">'+text3+'</text>'+
              '<text id="4">'+text21+'</text>'+
              '<text id="5">'+text22+'</text>'+
              '<text id="6">'+text31+'</text>'+
              '<text id="7">'+text32+'</text>'+
              '</binding>'+

            '</visual>'+
          '</tile>';
}