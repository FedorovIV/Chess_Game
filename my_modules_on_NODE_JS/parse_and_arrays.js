const createEmptyArray = () => {
    arr = new Array(64);
  
    for (i = 0; i < 64; i++) {
      arr[i] = 'None';
    }
  
  
    return arr;
  }
  
  function createStartPossion() {
    arr = new Array(64);
  
    for (i = 0; i < 64; i++) {
      arr[i] = 'None';
    }
  
    arr[0] = 'Brook';
    arr[1] = 'Bknight';
    arr[2] = 'Bbishop';
    arr[3] = 'Bking';
    arr[4] = 'Bqueen';
    arr[5] = 'Bbishop';
    arr[6] = 'Bknight';
    arr[7] = 'Brook';
  
    for (i = 0; i < 8; i++) {
      arr[8 + i] = 'Bpawn';
    }
  
    for (i = 0; i < 8; i++) {
      arr[8 + i] = 'Bpawn';
    }
  
    arr[56 + 0] = 'Wrook';
    arr[56 + 1] = 'Wknight';
    arr[56 + 2] = 'Wbishop';
    arr[56 + 3] = 'Wking';
    arr[56 + 4] = 'Wqueen';
    arr[56 + 5] = 'Wbishop';
    arr[56 + 6] = 'Wknight';
    arr[56 + 7] = 'Wrook';
  
    for (i = 0; i < 8; i++) {
      arr[48 + i] = 'Wpawn';
    }
  
    return arr;
  }
  
  //Parse data
  function makeNecessaryMassive(str) {
  
    if (str[0] == 'P') {
      var mydata = {
        NecessaryArray: null,
        strret: null,
      }
      
      return mydata;
    } else {
      arr = new Array(64);
      //console.log(str.substr(13))//.split(' ').map(Number));
  
      strWithotLetter = str.substr(13)
  
      a = 0;
      sign = 1;
      k = 0;
      flag = false;
      strret = '';
      for (i = 0; i < strWithotLetter.length; i++) {
        if (str[i] == '.')
        {
          //console.log(strWithotLetter.substr(i-1));
          if (strWithotLetter.substr(i-1).includes('E_WIN'))
          {
            strret = 'WHITE_WIN';
          }
          if (strWithotLetter.substr(i-1).includes('K_WIN'))
          {
            strret = 'BLACK_WIN';
          } 
          if (strWithotLetter.substr(i-1).includes('PIECE'))
          {
            strret = 'CHANGE_PIECE';
          }
          break;
        }
        c = strWithotLetter[i];
        if (flag && (c == '\n' || c == ' ')) {
  
          a = a * sign;
          arr[k] = a;
          k++;
          sign = 1;
          flag = false;
        }
        if (c == '-') {
          sign = -1;
        }
  
        if (c >= '0' && c <= '9') {
          a = Number(strWithotLetter[i]);
          flag = true;
        }
      }
  
      //console.log(arr);
  
      NecessaryArray = new Array(64);
  
      for (i = 0; i < 64; i++) {
        if (arr[i] > 0) {
          NecessaryArray[i] = 'W';
        }
        if (arr[i] < 0) {
          NecessaryArray[i] = 'B';
        }
  
        switch (Math.abs(arr[i])) {
          case 0:
            NecessaryArray[i] = 'None';
            break
          case 1:
            NecessaryArray[i] += 'king';
            break
          case 2:
            NecessaryArray[i] += 'queen';
            break
          case 3:
            NecessaryArray[i] += 'bishop';
            break
          case 4:
            NecessaryArray[i] += 'rook';
            break
          case 5:
            NecessaryArray[i] += 'knight';
            break
          case 6:
            NecessaryArray[i] += 'pawn';
            break
  
          default:
            NecessaryArray[i] = 'None';
        }
      }
      arr2 = new Array(64);
  
      for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
          arr2[56 - i * 8 + j] = NecessaryArray[8 * i + j];
        }
      }
  
      for (i = 0; i < 64; i++) {
        NecessaryArray[i] = arr2[i];
      }

      var mydata = {
        NecessaryArray: NecessaryArray,
        strret: strret,
      }

      return mydata;
    }
  }
  //Parse data for Sel Cells Array
  function makeSelCellsArray(str) {
    if (str[0] == 'P') {
      arr = new Array;
  
      //console.log(str.substr(14));
  
      CertainStr = str.substr(14);
  
      if (CertainStr[0] == '0') {
        return null;
      }
      else {
  
        flagNum = true;
        flagStart = false;
        flag = false;
        s = '';
        k = 0;
  
        for (i = 0; i < CertainStr.length; i++) {
  
  
          c = CertainStr[i];
          if (!(c >= '0' && c <= '9')){
            flagNum = false;
          }
          if (!flagNum && (c != ' ' &&  c != '\n')){
            flagStart = true;
          }
          if (flagStart){
            if (flag && (c == '\n' || c == ' ')){
              arr[k] = s;
              k++;
              s = '';
              flag = false
            }
  
            if ((c >= '0' && c <= '9')){
              flag = true;
              s+=c;
            }
  
            if ((c >= 'a' && c <= 'z')){
              s+=c;
            }
          }
  
        }
  
        return arr;
  
      }
    } else {
      return null;
    }
  }

  module.exports = {
    createEmptyArray: createEmptyArray,
    createStartPossion: createStartPossion,
    makeNecessaryMassive: makeNecessaryMassive,
    makeSelCellsArray: makeSelCellsArray,
  }