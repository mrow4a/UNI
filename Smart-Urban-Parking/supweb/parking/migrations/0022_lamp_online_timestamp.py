# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0021_auto_20150809_1552'),
    ]

    operations = [
        migrations.AddField(
            model_name='lamp',
            name='online_timestamp',
            field=models.DateTimeField(default=django.utils.timezone.now, blank=True),
        ),
    ]
