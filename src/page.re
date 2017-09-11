type action =
  | SetImage string
  | UploadImage string
  | RemoveImage
  | ToggleExpand;

/* The component's state type. It can be anything, including, commonly, being a record type */
type state = {
  image: option string,
  existingVideos: array string,
  isExpanded: bool
};

let component = ReasonReact.reducerComponent "App";

let uploadImage img _self => UploadImage img;

let ipcSend = Electron.IpcRenderer.send;

let ipcOn = Electron.IpcRenderer.on;

let sendCommand command _e => ipcSend command "foo";

external extname : string => string = "extname" [@@bs.module "path"];

let getVideos () =>
  Node_fs.readdirSync StoreFile.storeDir |> Array.to_list |>
  List.filter (fun f => extname f == ".webm") |>
  List.map (fun x => Node_path.join [|StoreFile.storeDir, x|]) |> Array.of_list;

let make _children => {
  ...component,
  initialState: fun () => {image: None, isExpanded: false, existingVideos: [||]},
  reducer: fun action state =>
    switch action {
    | UploadImage str =>
      ReasonReact.Update {
        ...state,
        image: Some str,
        existingVideos: Array.append [|str|] state.existingVideos
      }
    | SetImage str => ReasonReact.Update {...state, image: Some str}
    | RemoveImage => ReasonReact.Update {...state, image: None}
    | ToggleExpand =>
      ReasonReact.UpdateWithSideEffects
        {...state, isExpanded: not state.isExpanded} (sendCommand "expand-window")
    },
  didMount: fun self => {
    let existingVideos = getVideos ();
    ReasonReact.UpdateWithSideEffects
      {...self.state, existingVideos}
      (fun _ => ipcOn "capture-taken" (fun _e image => self.reduce (uploadImage image) ()))
  },
  render: fun self => {
    let stop = self.reduce (fun _e => RemoveImage);
    let className a => a ^ " " ^ (self.state.isExpanded ? a ^ "--open" : a ^ "-closed");
    <div className=(className "app")>
      (
        switch self.state.image {
        | Some img => <DelayUploadButton filename=img onCompleted=stop onCancelled=stop />
        | None =>
          <div className="start">
            <button className="start__button" onClick=(sendCommand "open-capture")>
              (ReasonReact.stringToElement {js|＋|js})
            </button>
            <div className="channelSelect">
              <button
                className="channelSelect__button" onClick=(self.reduce (fun _ => ToggleExpand))>
                (ReasonReact.stringToElement "#general")
                <span className="channelSelect__chevron">
                  (ReasonReact.stringToElement {js|▾|js})
                </span>
              </button>
            </div>
          </div>
        }
      )
      <div className=(className "addModule__channelList") />
      <ListVideos
        onVideoSelect=(self.reduce (fun img => SetImage img))
        videos=self.state.existingVideos
      />
    </div>
  }
};
