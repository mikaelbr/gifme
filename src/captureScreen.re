type recordStates =
  | BeforeRecord
  | CountdownRecord
  | IsRecording
  | DoneRecording;

type state = {
  videoRef: ref (option Dom.element),
  inState: recordStates
};

let setSectionRef theRef {ReasonReact.state: state} => state.videoRef := Js.Null.to_opt theRef;

type action =
  | SetStream Navigator.mediaStream
  | SetState recordStates;

let component = ReasonReact.reducerComponent "CaptureScreen";

let capturedButton onClick =>
  <button onClick className="capture__record"> (ReasonReact.stringToElement {js|◉|js}) </button>;

let make ::onComplete _children => {
  ...component,
  initialState: fun () => {videoRef: ref None, inState: BeforeRecord},
  reducer: fun action state =>
    switch action {
    | SetState status => ReasonReact.Update {...state, inState: status}
    | SetStream stream =>
      switch !state.videoRef {
      | None => ()
      | Some r =>
        Video.setSrcObject r stream;
        Video.onLoaded r (fun _ => Video.play r)
      };
      ReasonReact.NoUpdate
    },
  didMount: fun self => {
    let _f =
      Navigator.getUserMedia {"audio": false, "video": Navigator.Bool true} |>
      Js.Promise.then_ (
        fun stream => {
          self.reduce (fun _ => SetStream stream) ();
          Js.Promise.resolve ()
        }
      );
    ReasonReact.NoUpdate
  },
  render: fun self => {
    let onRecordStart = self.reduce (fun _ => SetState CountdownRecord);
    let onCountDownDone = self.reduce (fun _ => SetState IsRecording);
    let stopRecording = self.reduce (fun _ => SetState DoneRecording);
    switch self.state.inState {
    | DoneRecording =>
      <div className="capture__doneRecording">
        <h1> (ReasonReact.stringToElement "Is Recording") </h1>
        <button onClick=(fun _ => onComplete ())>
          (ReasonReact.stringToElement "Send recording")
        </button>
      </div>
    | x =>
      <div>
        <video ref=(self.handle setSectionRef) />
        (
          switch x {
          | BeforeRecord => capturedButton onRecordStart
          | CountdownRecord =>
            <div className="capture__countdown"> <Countdown onCompleted=onCountDownDone /> </div>
          | IsRecording =>
            <div className="capture__isRecording">
              <button className="capture__stopButton" onClick=stopRecording>
                (ReasonReact.stringToElement {js|▣|js})
              </button>
            </div>
          | _ => <div />
          }
        )
      </div>
    }
  }
};
