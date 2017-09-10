let ipcSend = Electron.IpcRenderer.send;

let sendCapture data => ipcSend "capture-taken" data;

let onClose _e => ipcSend "close-capture" "";

let component = ReasonReact.statelessComponent "Capture";

let closeButton onClick =>
  <button onClick className="capture__close"> (ReasonReact.stringToElement {js|✖|js}) </button>;

let make _children => {
  ...component,
  render: fun _self => <div> (closeButton onClose) <CaptureScreen onComplete=sendCapture /> </div>
};
