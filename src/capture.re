let ipcSend = Electron.IpcRenderer.send Electron.ipcRenderer;

let sendCapture _e => ipcSend "capture-taken" "capture content here";

let onClose _e => ipcSend "close-capture" "";

type action =
  | SetStream Navigator.mediaStream;

let component = ReasonReact.statelessComponent "Capture";

let closeButton onClick =>
  <button onClick className="capture__close"> (ReasonReact.stringToElement {js|✖|js}) </button>;

let make _children => {
  ...component,
  render: fun _self => <div> (closeButton onClose) <CaptureScreen onComplete=sendCapture /> </div>
};
