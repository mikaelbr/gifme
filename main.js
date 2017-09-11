const electron = require('electron');
const { app, BrowserWindow, ipcMain } = electron;

const path = require('path');
const url = require('url');
const fs = require('fs');

// Keep a global reference of the window object, if you don't, the window will
// be closed automatically when the JavaScript object is garbage collected.
let mainWindow;
let captureWindow;

const minHeight = 300;
const maxHeight = 600;
const width = 325;

let storePath = path.join(__dirname, 'stored-videos');

function getName() {
  return String(new Date().getTime());
}

function storeFile(buffer) {
  console.log(buffer);
  // const buffer = new Buffer(binaryString);
  const file = path.join(storePath, getName()) + '.webm';
  fs.writeFile(file, buffer, {}, (err, res) => {
    if (err) {
      console.error(err);
      return;
    }
    console.log('video saved');
  });
}

function createWindow() {
  // Create the browser window.
  mainWindow = new BrowserWindow({
    backgroundColor: '#53A0FD',
    width,
    minHeight,
    height: minHeight,
    maxHeight,
    vibrancy: 'appearance-based',
    frame: false
  });

  // and load the index.html of the app.
  mainWindow.loadURL(
    url.format({
      pathname: path.join(__dirname, 'index.html'),
      protocol: 'file:',
      slashes: true
    })
  );

  // Open the DevTools.
  // mainWindow.webContents.openDevTools()

  // Emitted when the window is closed.
  mainWindow.on('closed', function() {
    if (process.platform !== 'darwin') {
      app.quit();
    }
    // Dereference the window object, usually you would store windows
    // in an array if your app supports multi windows, this is the time
    // when you should delete the corresponding element.
    mainWindow = null;
  });
}

ipcMain.on('open-capture', (event, arg) => {
  captureWindow = new BrowserWindow({
    backgroundColor: '#000',
    width: 640,
    height: 480,
    frame: false,
    resizable: false,
    parent: mainWindow,
    modal: true
  });

  ipcMain.on('capture-taken', (_, arg) => {
    if (captureWindow) {
      captureWindow.close();
    }
    // storeFile(arg);
    event.sender.send('capture-taken', arg);
  });

  ipcMain.on('close-capture', (_, arg) => {
    if (captureWindow) {
      captureWindow.close();
    }
  });

  // and load the index.html of the app.
  captureWindow.loadURL(
    url.format({
      pathname: path.join(__dirname, 'capture.html'),
      protocol: 'file:',
      slashes: true
    })
  );

  // Emitted when the window is closed.
  captureWindow.on('closed', function() {
    ipcMain.removeAllListeners('capture-taken');
    ipcMain.removeAllListeners('close-capture');
    captureWindow = null;
  });
});

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on('ready', createWindow);

// Quit when all windows are closed.
app.on('window-all-closed', function() {
  // On OS X it is common for applications and their menu bar
  // to stay active until the user quits explicitly with Cmd + Q
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', function() {
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (mainWindow === null) {
    createWindow();
  }
});

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
