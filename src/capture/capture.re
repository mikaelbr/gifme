let ipcSendBuffer = Electron.IpcRenderer.sendBuffer;

let ipcSend = Electron.IpcRenderer.send;

let storeFile data => {
  let filename = StoreFile.filename ();
  let reader = FileReader.create ();
  FileReader.onload
    reader
    (
      fun () => {
        let result = FileReader.resultAsString reader;
        Node_fs.writeFileSync filename result `binary;
        ()
      }
    );
  FileReader.readAsBinaryString reader data;
  filename
};

let sendCapture data => {
  ipcSend "capture-taken" (storeFile data);
  ()
};

let onClose _e => ipcSend "close-capture" "";

let component = ReasonReact.statelessComponent "Capture";

let closeButton onClick =>
  <button onClick className="capture__close"> (ReasonReact.stringToElement {js|✖|js}) </button>;

let make _children => {
  ...component,
  render: fun _self => <div> (closeButton onClose) <CaptureScreen onComplete=sendCapture /> </div>
};
