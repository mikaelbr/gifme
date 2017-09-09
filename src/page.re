type action =
  | SetImage string
  | RemoveImage;

/* The component's state type. It can be anything, including, commonly, being a record type */
type state = {image: option string};

let component = ReasonReact.reducerComponent "App";

let setImage img _self => SetImage img;

let ipcSend = Electron.IpcRenderer.send Electron.ipcRenderer;

let ipcOn = Electron.IpcRenderer.on Electron.ipcRenderer;

let sendCommand command _e => ipcSend command "foo";

let make _children => {
  ...component,
  initialState: fun () => {image: None},
  reducer: fun action _state =>
    switch action {
    | SetImage str => ReasonReact.Update {image: Some str}
    | RemoveImage => ReasonReact.Update {image: None}
    },
  didMount: fun self => {
    ipcOn "capture-taken" (fun _e image => self.reduce (setImage image) ());
    ReasonReact.NoUpdate
  },
  render: fun self => {
    let isUploading =
      switch self.state.image {
      | Some _ => true
      | None => false
      };
    let stop = self.reduce (fun _e => RemoveImage);
    if isUploading {
      <DelayUploadButton onCompleted=stop onCancelled=stop />
    } else {
      <div className="start">
        <button className="start__button" onClick=(sendCommand "open-capture")>
          (ReasonReact.stringToElement {js|＋|js})
        </button>
        <div className="channelSelect">
          <button className="channelSelect__button" onClick=(sendCommand "expand-window")>
            (ReasonReact.stringToElement "#general")
            <span className="channelSelect__chevron">
              (ReasonReact.stringToElement {js|▾|js})
            </span>
          </button>
        </div>
      </div>
    }
  }
};
