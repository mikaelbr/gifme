type recordStates =
  | BeforeRecord
  | CountdownRecord
  | IsRecording
  | DoneRecording;

type state = {
  videoRef: ref (option Dom.element),
  recordStream: option MediaRecorder.mediaRecorder,
  inState: recordStates,
  chunk: option Blob.t
};

let setSectionRef theRef {ReasonReact.state: state} => state.videoRef := Js.Null.to_opt theRef;

type action =
  | RecordingFinished
  | SetStream Navigator.mediaStream MediaRecorder.mediaRecorder
  | SetState recordStates
  | SetRecord bool
  | Reset
  | UpdateChunks Blob.t;

let reducer action state =>
  switch action {
  | Reset => ReasonReact.Update {...state, chunk: None, inState: BeforeRecord}
  | RecordingFinished => ReasonReact.SideEffects (fun _self => Js.log state.chunk)
  | UpdateChunks chunk => ReasonReact.Update {...state, chunk: Some chunk}
  | SetState status => ReasonReact.Update {...state, inState: status}
  | SetRecord active =>
    ReasonReact.SideEffects (
      fun _self =>
        switch (active, state.recordStream) {
        | (true, Some s) => MediaRecorder.start s
        | (false, Some s) => MediaRecorder.stop s
        | _ => ()
        }
    )
  | SetStream stream m =>
    ReasonReact.UpdateWithSideEffects
      {...state, recordStream: Some m}
      (
        fun _self =>
          switch !state.videoRef {
          | None => ()
          | Some r =>
            Video.setSrcObject r stream;
            Video.onLoaded r (fun _ => Video.play r)
          }
      )
  };

let component = ReasonReact.reducerComponent "CaptureScreen";

let capturedButton onClick =>
  <button onClick className="capture__record"> (ReasonReact.stringToElement {js|◉|js}) </button>;

let startUserMedia (self: ReasonReact.self state ReasonReact.noRetainedProps action) => {
  let _f =
    Navigator.getUserMedia {"audio": false, "video": Navigator.Bool true} |>
    Js.Promise.then_ (
      fun stream => {
        let m = MediaRecorder.create_media_recorder stream;
        MediaRecorder.ondataavailable m (fun d => self.reduce (fun _ => UpdateChunks d##data) ());
        MediaRecorder.onstop m (fun _ => self.reduce (fun _ => RecordingFinished) ());
        self.reduce (fun _ => SetStream stream m) ();
        Js.Promise.resolve ()
      }
    );
  ()
};

let make ::onComplete _children => {
  ...component,
  initialState: fun () => {
    videoRef: ref None,
    inState: BeforeRecord,
    recordStream: None,
    chunk: None
  },
  reducer,
  didMount: fun _self => ReasonReact.SideEffects startUserMedia,
  render: fun self => {
    let onRecordStart = self.reduce (fun _ => SetState CountdownRecord);
    let onCountDownDone _ => {
      self.reduce (fun _ => SetState IsRecording) ();
      self.reduce (fun _ => SetRecord true) ()
    };
    let stopRecording _ => {
      self.reduce (fun _ => SetState DoneRecording) ();
      self.reduce (fun _ => SetRecord false) ()
    };
    let reset _ => self.reduce (fun _ => Reset) ();
    switch self.state.inState {
    | DoneRecording =>
      switch self.state.chunk {
      | Some chunk => <PreviewConfirm chunk onComplete onCancel=reset />
      | _ => <h1> (ReasonReact.stringToElement "Nope") </h1>
      }
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
