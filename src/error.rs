use std::error::Error;
use std::fmt;

pub type MrbResult<T> = Result<T, MrbError>;

#[derive(Clone, Debug)]
pub struct MrbError {
    kind: ErrorKind,
    context: Vec<String>,
}

impl MrbError {
    pub fn chain<S: AsRef<str>>(mut self, s: S) -> Self {
        let s = s.as_ref().to_owned();
        self.context.push(s);
        self
    }
}

impl fmt::Display for MrbError {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "kind: {}", self.kind.as_str())?;
        for (i, s) in self.context.iter().enumerate() {
            write!(f, "{:3}: {}", i, s)?;
        }
        Ok(())
    }
}

impl Error for MrbError {
    fn description(&self) -> &str {
        self.kind.as_str()
    }
}

impl From<ErrorKind> for MrbError {
    fn from(e: ErrorKind) -> MrbError {
        MrbError {
            kind: e,
            context: Vec::new(),
        }
    }
}

macro_rules! chain {
    ($e:expr, $msg:expr,) => {
        chain!($e, $msg)
    };
    ($e:expr, $msg:expr) => {{
        match $e {
            Ok(t) => t,
            Err(e) => return Err(e.chain($msg)),
        }
    }};
    ($e:expr, $fmt:expr, $($arg:tt)+) => {{
        match $e {
            Ok(t) => t,
            Err(e) => return Err(e.chain(format!($fmt, $($arg)+))),
        }
    }};
}

macro_rules! get_ref {
    ($ptr:expr) => {
        if let Some(r) = $ptr.as_mut() {
            r
        } else {
            return Err(ErrorKind::Null.into());
        }
    };
    ($ptr:expr,) => {
        get_ref!($ptr);
    };
    ($ptr:expr, $msg:expr) => {
        if let Some(r) = $ptr.as_mut() {
            r
        } else {
            return Err(ErrorKind::Null.into_with($msg));
        }
    };
    ($ptr:expr, $fmt:expr, $($arg:tt)+) => {
         if let Some(r) = $ptr.as_mut() {
            r
        } else {
            return Err(ErrorKind::Null.into_with(format!($fmt, $($arg)+)));
        }
    };
}

macro_rules! non_null {
     ($ptr:expr) => {
        if let Some(r) = NonNull::new($ptr) {
            r
        } else {
            return Err(ErrorKind::Null.into());
        }
    };
    ($ptr:expr,) => {
        get_ref!($ptr);
    };
    ($ptr:expr, $msg:expr) => {
        if let Some(r) = NonNull::new($ptr) {
            r
        } else {
            return Err(ErrorKind::Null.into_with($msg));
        }
    };
    ($ptr:expr, $fmt:expr, $($arg:tt)+) => {
         if let Some(r) = NonNull::new($ptr) {
            r
        } else {
            return Err(ErrorKind::Null.into_with(format!($fmt, $($arg)+)));
        }
    };
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum ErrorKind {
    FreeVal,
    Null,
    NumCast,
    OtherType,
    Undefined,
}

impl ErrorKind {
    fn as_str(&self) -> &str {
        use self::ErrorKind::*;
        match self {
            FreeVal => "FreeVal(invalid access to already freed variable)",
            Null => "Null(access to null pointer)",
            NumCast => "NumCast error(not mruby problem)",
            OtherType => "Other variant found when conveting from MrbValue",
            Undefined => "Undefined(access to undefined value or methods)",
        }
    }
    pub fn into_with<S: AsRef<str>>(self, s: S) -> MrbError {
        let s = s.as_ref().to_owned();
        MrbError {
            kind: self,
            context: vec![s],
        }
    }
}
