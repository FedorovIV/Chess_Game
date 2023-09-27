const { app, BrowserWindow, ipcMain, dialog, Menu } = require('electron')
const child_process = require('node:child_process');
const { create } = require('node:domain');
const fs = require('node:fs');
const parse_and_arrays = require('./my_modules_on_NODE_JS/parse_and_arrays');

const path = require('path');

//Создание рендера
function createWindow() {
  const mainWindow = new BrowserWindow({
    icon: 'images/Bking.png',
    contextIsolation: false,
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js')
    }
  })

  mainWindow.loadFile('index.html')

  return mainWindow;
}


app.whenReady().then(() => {

  const child_process = require('node:child_process');

  const subProcessOnCPP = child_process.spawn('CPP.exe');

  //Organization of communication between processes///////////////
  //Array of figures
  var ChessMassive = parse_and_arrays.createEmptyArray();
  //var ChessMassive = createEmptyArray();

  flagGameBegin = false;

  //On-event. Hadnler Start Button
  ipcMain.handle('dialog:StartButton', () => {

    subProcessOnCPP.stdin.write("show" + '\n');
    // arr = parse_and_arrays.createStartPossion();

    // for (i = 0; i < 64; i++) {
    //   ChessMassive[i] = arr[i];
    // }

    flagGameBegin = true;
    // return ChessMassive;
  });

  //On-event. Handler Cell Event
  //Write Cell id to CPP process
  ipcMain.on('CellEvent', (event, CellId) => {
    if (!flagGameBegin) {
      return parse_and_arrays.createEmptyArray();
      //return createEmptyArray();
    } else {
      console.log(CellId);
      subProcessOnCPP.stdin.write(CellId + '\n');
      return 0;
    }
  });

  ipcMain.on('ButBackEvent', (event) => {
    subProcessOnCPP.stdin.write("previous" + '\n');
  });

  ipcMain.on('ButNextEvent', (event) => {
    subProcessOnCPP.stdin.write("next" + '\n');
  });

  ipcMain.on('ChooseFigureEvent', (event, FigureName) => {
      subProcessOnCPP.stdin.write('choosePiece'+'\n'+FigureName+'\n'+'show'+'\n');
  });

  mainWindow = createWindow();

  //On-event. Send array of figures (ChessMassive)
  subProcessOnCPP.stdout.on('data', (data) => {

    console.log(data.toString('utf8'));
    str = data.toString('utf8');

    var mydata = parse_and_arrays.makeNecessaryMassive(str);

    FiguresArray = mydata.NecessaryArray;

    //console.log(FiguresArray);
    strret = mydata.strret;
    
    SelCellsArray = parse_and_arrays.makeSelCellsArray(str);
    //SelCellsArray = null;
    MoveMaked = false;

    if (FiguresArray == null) {
      mainWindow.webContents.send('update-board', ChessMassive, SelCellsArray, MoveMaked);
    } else {
      for (i = 0; i < 64; i++) {
        ChessMassive[i] = FiguresArray[i];
      }

      MoveMaked = true;
      mainWindow.webContents.send('update-board', ChessMassive, null, MoveMaked);
    }
    if (strret == 'WHITE_WIN'){
      mainWindow.webContents.send('end-game', 'WHITE_WIN');
    }
    if (strret == 'BLACK_WIN'){
      mainWindow.webContents.send('end-game', 'BLACK_WIN');
    }
    if (strret == 'CHANGE_PIECE'){
      mainWindow.webContents.send('change-piece');
    }
  });
  //////////////////////////////////////////////////////////////////////
  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})
//Завершение работы программы
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

