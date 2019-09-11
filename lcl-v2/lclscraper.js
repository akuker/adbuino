/**
 * @name LCL
 *
 * @desc Logs into LCL. Provide your username and password as environment variables when running the script, i.e:
 * `LCL_ID=myuser LCL_CODE=mypassword node lcltest.js`
 *
 */
const puppeteer  = require('puppeteer')
var   Tesseract  = require('tesseract.js')
var   fs         = require('fs');
const screenshot = 'lcl.png';

(async () => {
  const browser = await puppeteer.launch({headless: true, args: ['--no-sandbox', '--disable-setuid-sandbox']});
  const page = await browser.newPage();
  await page.setViewport({ width: 1000, height: 1000 });
  await page.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.48 Safari/537.36 Edg/74.1.96.24");
  
  // Go to the base page and click on the connection icon as hitting the login page directly just redirects us
  await page.goto("https://lcl.fr");
  const [response] = await Promise.all([
    page.waitForNavigation(),
    page.click("a[href$='Accueil/']"),
  ]);

  // We're normally on the login page, https://particuliers.secure.lcl.fr/outil/UAUT?from=/outil/UWHO/Accueil/
  await page.screenshot({path: "page.png"});
  await page.type('#identifiantIdForm', process.env.LCL_ID.toString());

  // Can't directly type the code as the field is disabled.
  // Cherry on top, the field only accepts a transformed version of the code, randomized through the virtual keyboard.
  const rect = await page.evaluate(selector => {
    const element = document.querySelector(selector);
    const {x, y, width, height} = element.getBoundingClientRect();
    return {left: x, top: y, width, height, id: element.id};
  }, '#idImageClavier');
  
  await page.screenshot({
    path: 'keyboard.png',
    clip: {
      x: rect.left,
      y: rect.top,
      width: rect.width,
      height: rect.height
    }
  });
  await page.screenshot({path: "page.png"});

  // Just OCR the damn keyboard
  ocrResult = await Tesseract.recognize('keyboard.png');
  map = ocrResult.text.split(/ /)[0].replace(/[^\d]/g, '').trim();

  console.log('Current horizontal virtual keyboard map:', map);

  // Click on the areas matching the numbers we received, according to the code in env
  // DOM IDs are idImageClavier_01-10, going by column:
  // 1 3 5 7 9
  // 2 4 6 8 10
  code = process.env.LCL_CODE.toString();

  // Rearrange the keyboard map to match the DOM IDs:
  map = map.charAt(0)+map.charAt(5)+map.charAt(1)+map.charAt(6)+map.charAt(2)+map.charAt(7)+map.charAt(3)+map.charAt(8)+map.charAt(4)+map.charAt(9);
  console.log('Rearranged virtual keyboard map:', map);

  // Go through code character by character, find its location on the map and hit the matching area
  while (code.length > 0) {
    char = code.charAt(0);
    code = code.slice(1);
    
    posInMap = map.indexOf(char);
    posInMap++; //DOM IDs start at 1

    if (posInMap !== 10)
      posInMap = "0"+posInMap;

    //console.log("Clicking on "+ char +" at #idImageClavier_"+posInMap);
    await page.evaluate(selector=> {
      $(selector).click();
    },'#idImageClavier_'+posInMap);

  }

  inputClavier = await page.evaluate(()=> {return $("#postClavier").val()});

  await page.screenshot({ path: "beforelogin.png" });
  await page.click('#validerClavierCLI');
  await page.waitForNavigation(); // Cover potential redirections..
  await page.waitForNavigation();
  //await page.waitForNavigation();
  await page.screenshot({ path: "afterlogin.png" });

  // Time to do some API scraping
  await page.goto('https://particuliers.secure.lcl.fr/outil/UWSP/Synthese?mobile=true');
  await page.content(); 
  
  // We receive horribly mangled JSON/HTML, clean it up
  accountJson = await page.evaluate(() =>  {
      cleanedJson = document.body.textContent.replace(/\r?\n|\r/g,"");
      return JSON.parse(cleanedJson); 
  }); 

  // Function to get all transactions from an account in a clean JSON
  async function getTransactions(code,account,key) {
    await page.goto('https://particuliers.secure.lcl.fr/outil/UWLM/ListeMouvementsPar/accesListeMouvementsPar?agence='+
      code + '&compte=' + account+key + '&lettreCle=' + key + '&mobile=true');

    await page.content(); 
    transactionsJson = await page.evaluate(() =>  {
      cleanedJson = document.body.textContent.replace(/\r?\n|\r/g,"");
      return JSON.parse(cleanedJson); 
    }); 

    // Account API structure: listeMvt - > tableau d'opérations -> objets json contenant libelle, montant, etc
    // Just use the bank's transaction list items as-is
    return transactionsJson.listeMvt;
  };

  // Prepare array for the final big JSON result
  var result = new Array(0);

  //Iterate through account listing and build JSON for each acct
  //account listing structure : famille -> tableau de 4 types de comptes, pro, particulier, épargne, autres
  //chaque type de compte: libelle,listGroupe -> libelle/listCompte -> les Comptes
  for(var i in accountJson.famille) {
      
    var type = accountJson.famille[i];
    console.log("-----------");
    console.log(type.listGroupe[0].libelle + ":");

    for (var j in type.listGroupe[0].listCompte) {
      var compte = type.listGroupe[0].listCompte[j];
      console.log("---" + compte.natureLibelle + 
                  " --- CODE: " + compte.agence + "-" + compte.compte + compte.lettreCle + 
                  "  --- SOLDE: " + compte.soldeDisponible + " EUR");

      var account = {label:compte.natureLibelle, 
                     total:compte.soldeDisponible, 
                     code:compte.agence + "-" + compte.compte + compte.lettreCle};
      account.transactions = await getTransactions(compte.agence, compte.compte, compte.lettreCle);
      console.log("Pushing for "+compte.compte);
      result.push(account);
    }
  }

  //Publish result to .json file and close browser
  var json = JSON.stringify(result);
  fs.writeFileSync('lcl.json', json); 

  browser.close();
  process.exit(0);
})()
