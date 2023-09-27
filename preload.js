const { contextBridge, ipcRenderer } = require('electron')

contextBridge.exposeInMainWorld('electronAPI',{

  CellEvent: (CellId) => ipcRenderer.send('CellEvent', CellId),
  StartButton: () =>  ipcRenderer.invoke('dialog:StartButton'),
  UpdateBoard: (callback) => ipcRenderer.on('update-board', callback),
  ButtonBack: () => ipcRenderer.send('ButBackEvent'),
  ButtonNext: () => ipcRenderer.send('ButNextEvent'),
  ChooseFigure:  (FigureName) => ipcRenderer.send('ChooseFigureEvent', FigureName),
  EndGame: (callback) => ipcRenderer.on('end-game', callback),
  ChangePiece: (callback) => ipcRenderer.on('change-piece', callback),
})

