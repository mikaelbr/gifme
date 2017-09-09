let component = ReasonReact.statelessComponent "CaptureScreen";

let ipcSend = Electron.IpcRenderer.send Electron.ipcRenderer;

let onClick _e => ipcSend "capture-taken" "capture content here";

let make _children => {
  ...component,
  render: fun _self =>
    <h1> <button onClick> (ReasonReact.stringToElement "Send image!") </button> </h1>
};
