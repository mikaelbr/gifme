let tickTimeInMs = 10;

let msInSec = 1000;

let seconds = 10;

let rest = msInSec / tickTimeInMs;

type action =
  | Tick;

/* The component's state type. It can be anything, including, commonly, being a record type */
type state = {
  total: int,
  timerId: ref (option Js.Global.intervalId)
};

let component = ReasonReact.reducerComponent "Counter";

let button text percentage onClick =>
  <button className="upload" onClick>
    <p className="upload__tagline">
      (ReasonReact.stringToElement "Uploading to #general in...")
    </p>
    <h1 className="upload__text"> (ReasonReact.stringToElement text) </h1>
    <h1 className="upload__cancel"> (ReasonReact.stringToElement "Cancel upload") </h1>
    <div
      className="upload__progress"
      style=(ReactDOMRe.Style.make transform::("scaleX(" ^ string_of_float percentage ^ ")") ())
    />
  </button>;

let make ::filename ::onCancelled ::onCompleted ::seconds=seconds _children => {
  ...component,
  initialState: fun () => {total: seconds * rest * tickTimeInMs, timerId: ref None},
  reducer: fun action state =>
    switch action {
    | Tick =>
      if (state.total <= 0) {
        switch !state.timerId {
        | Some id => Js.Global.clearInterval id
        | _ => ()
        };
        onCompleted ();
        ReasonReact.NoUpdate
      } else {
        ReasonReact.Update {...state, total: state.total - tickTimeInMs}
      }
    },
  didMount: fun self => {
    self.state.timerId := Some (Js.Global.setInterval (self.reduce (fun _ => Tick)) tickTimeInMs);
    ReasonReact.NoUpdate
  },
  willUnmount: fun self =>
    switch !self.state.timerId {
    | Some id => Js.Global.clearInterval id
    | _ => ()
    },
  render: fun {state: {total}} => {
    let seconds = total / 1000;
    let percentage = float_of_int (total / 100) /. 100.0;
    let timesMessage = seconds == 1 ? "second" : "seconds";
    let text = {j|$seconds $timesMessage|j};
    let click _e => {
      Js.log "dsadsa";
      onCancelled ();
      ()
    };
    <div className="container">
      <div className="preview"> <video src=filename autoPlay=Js.true_ loop=Js.true_ /> </div>
      (
        if (seconds <= 0) {
          <div className="uploaded">
            <p className="uploaded__text">
              (ReasonReact.stringToElement "Your gif has been uploaded")
            </p>
          </div>
        } else {
          button text percentage click
        }
      )
    </div>
  }
};
