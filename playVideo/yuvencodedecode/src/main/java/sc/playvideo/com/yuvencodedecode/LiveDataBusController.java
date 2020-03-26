package sc.playvideo.com.yuvencodedecode;

import android.arch.lifecycle.LifecycleOwner;
import android.arch.lifecycle.Observer;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.annotation.Nullable;
import android.text.TextUtils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;



/**
 * Created by hjt on 2019/12/13
 */
public class LiveDataBusController {
    public interface LiveDataBusCallBack {
        boolean handleMessage(Message msg);
    }

    private Handler handler = new Handler(Looper.getMainLooper());
    private Map<LiveDataBusCallBack, Observer> map = new HashMap<>();
    private static LiveDataBusController mLiveDataBusController = null;

    public static LiveDataBusController getInstance() {
        if (mLiveDataBusController == null) {
            synchronized (LiveDataBusController.class) {
                if (mLiveDataBusController == null) {
                    mLiveDataBusController = new LiveDataBusController();
                }
            }
        }
        return mLiveDataBusController;
    }

    private void addRegister(String tag, final LiveDataBusCallBack data) {
        if (!TextUtils.isEmpty(tag) && data != null) {
            synchronized (map) {
                Observer observer = map.remove(data);
                if (observer != null) {
                    LiveDataBus.get().with(tag, Message.class).removeObserver(observer);
                }
                observer = new Observer<Message>() {
                    @Override
                    public void onChanged(@Nullable Message msg) {
                        data.handleMessage(msg);
                    }
                };
                LiveDataBus.get().with(tag, Message.class).observeForever(observer);
                map.put(data, observer);

            }
        }
    }

    public void addRegister(String tag, final LiveDataBusCallBack data, LifecycleOwner owner) {
        if (!TextUtils.isEmpty(tag) && data != null) {
            if (owner == null) {
                //无生命周期
                addRegister(tag, data);
            } else {
                synchronized (map) {
                    Observer observer = map.remove(data);
                    if (observer != null) {
                        LiveDataBus.get().with(tag, Message.class).removeObserver(observer);
                    }
                    observer = new Observer<Message>() {
                        @Override
                        public void onChanged(@Nullable Message msg) {
                            data.handleMessage(msg);
                        }
                    };
                    LiveDataBus.get().with(tag, Message.class).observe(owner, observer);
                    map.put(data, observer);
                }
            }
        }
    }

    public void removeRegister(String tag, LiveDataBusCallBack data, LifecycleOwner owner) {
        if (!TextUtils.isEmpty(tag) && data != null) {
            if (owner == null) {
                removeRegister(tag, data);
            } else {
                synchronized (map) {
                    map.remove(data);
                }
            }
        }
    }

    private void removeRegister(String tag, LiveDataBusCallBack data) {
        if (!TextUtils.isEmpty(tag) && data != null) {
            synchronized (map) {
                Observer observer = map.remove(data);
                if (observer != null) {
                    LiveDataBus.get().with(tag, Message.class).removeObserver(observer);
                }
            }
        }
    }

    public void sendBusMessage(String channel, Message msg) {
        synchronized (map) {
            if (!TextUtils.isEmpty(channel) && msg != null) {
                if (Looper.myLooper() == Looper.getMainLooper()) {
                    LiveDataBus.get().with(channel).setValue(msg);
                } else {
                    handler.post(MsgRunnable.creatMsgRunnable(channel, msg));
                }
            }
        }
    }

    public void sendBusMessage(List<String> channel, Message msg) {
        if (channel != null && channel.size() > 0) {
            synchronized (map) {
                for (String tag : channel) {
                    if (!TextUtils.isEmpty(tag) && msg != null) {
                        if (Looper.myLooper() == Looper.getMainLooper()) {
                            LiveDataBus.get().with(tag).setValue(msg);
                        } else {
                            handler.post(MsgRunnable.creatMsgRunnable(tag, msg));
                        }
                    }
                }
            }
        }
    }

    public List<String> creatChannelList(String... str) {
        ArrayList<String> list = new ArrayList<>();
        for (String s : str) {
            if (!TextUtils.isEmpty(s)) {
                list.add(s);
            }
        }
        return list;
    }

    public static class MsgRunnable implements Runnable {
        String tag;
        Message msg;

        public static MsgRunnable creatMsgRunnable(String tag, Message msg) {
            return new MsgRunnable(tag, msg);
        }

        public MsgRunnable(String tag, Message msg) {
            this.tag = tag;
            this.msg = msg;
        }

        @Override
        public void run() {
            LiveDataBus.get().with(tag).setValue(msg);
        }
    }
}
