type recordStatus =
  | BeforeRecord
  | CountdownRecord
  | IsRecording
  | DoneRecording;

type state = {
  stream: option Navigator.mediaStream,
  recordStream: option MediaRecorder.t,
  status: recordStatus,
  chunk: option Blob.t
};

type action =
  | RecordingFinished
  | SetStream Navigator.mediaStream MediaRecorder.t
  | SetStatus recordStatus
  | Reset
  | UpdateChunks Blob.t;

let reducer action state =>
  switch action {
  | Reset => ReasonReact.Update {...state, chunk: None, status: BeforeRecord}
  | RecordingFinished =>
    ReasonReact.SideEffects (
      fun self =>
        switch self.state.recordStream {
        | Some s => MediaRecorder.stop s
        | _ => ()
        }
    )
  | UpdateChunks chunk => ReasonReact.Update {...state, chunk: Some chunk}
  | SetStream stream m => ReasonReact.Update {...state, recordStream: Some m, stream: Some stream}
  | SetStatus status =>
    ReasonReact.UpdateWithSideEffects
      {...state, status}
      (
        fun self =>
          switch (status, self.state.recordStream) {
          | (IsRecording, Some s) => MediaRecorder.start s
          | _ => ()
          }
      )
  };

let component = ReasonReact.reducerComponent "CaptureScreen";

let capturedButton onClick =>
  <button onClick className="capture__record"> (ReasonReact.stringToElement {js|◉|js}) </button>;

let id i _ => i;

let make ::onComplete _children => {
  ...component,
  initialState: fun () => {status: BeforeRecord, recordStream: None, stream: None, chunk: None},
  reducer,
  render: fun self => {
    let r a => self.reduce (id a);
    let reset = r Reset;
    let onData d => r (UpdateChunks d) ();
    let onStop = r (SetStatus DoneRecording);
    let stopRecording = r RecordingFinished;
    let onCountDownDone = r (SetStatus IsRecording);
    let startRecording = r (SetStatus CountdownRecord);
    let onStreamReady stream m => r (SetStream stream m) ();
    switch self.state.status {
    | DoneRecording =>
      switch self.state.chunk {
      | Some chunk => <PreviewConfirm chunk onComplete onCancel=reset />
      | _ => <h1> (ReasonReact.stringToElement "Nope") </h1>
      }
    | status =>
      <div>
        <VideoStream onData onStop onStreamReady />
        (
          switch status {
          | BeforeRecord => capturedButton startRecording
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
