type state = {videoRef: ref (option Dom.element)};

let setSectionRef theRef {ReasonReact.state: state} => state.videoRef := Js.Null.to_opt theRef;

let component = ReasonReact.reducerComponent "VideoStream";

let refreshStream stream videoRef =>
  switch stream {
  | None => ()
  | Some s =>
    switch !videoRef {
    | None => ()
    | Some r =>
      Video.setSrcObject r s;
      Video.onLoaded r (fun _ => Video.play r)
    }
  };

let startUserMedia onData onStop onStreamReady {ReasonReact.state: state} => {
  let _f =
    Navigator.getUserMedia {"audio": false, "video": Navigator.Bool true} |>
    Js.Promise.then_ (
      fun stream => {
        let m = MediaRecorder.create stream;
        MediaRecorder.ondataavailable m (fun d => onData d##data);
        MediaRecorder.onstop m onStop;
        onStreamReady stream m;
        refreshStream (Some stream) state.videoRef;
        Js.Promise.resolve ()
      }
    );
  ()
};

let make ::onData ::onStop ::onStreamReady _children => {
  ...component,
  initialState: fun () => {videoRef: ref None},
  didMount: fun _self => ReasonReact.SideEffects (startUserMedia onData onStop onStreamReady),
  reducer: fun () _state => ReasonReact.NoUpdate,
  render: fun self => <video ref=(self.handle setSectionRef) />
};
